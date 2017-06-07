#include "ch.h"
#include "hal.h"
#include "hal_ad7124.h"
#include "sysParam.h"
//#define REG_READBACK

static SPIDriver *spiDev;
thread_t *thdAD7124;
static uint8_t nofChannel;
static uint8_t chCount;
static uint8_t chrrentCh;
static uint16_t vBiasEn = 0x0;
#define fVolts0 (2.5/268435456)

#define NOF_EN_CHANNELS 8

#define SPI_SELECT() palClearPad(GPIOB,9)
#define SPI_DESELECT() palSetPad(GPIOB,9);

static const SPIConfig hs_spicfg = {
  NULL,
  NULL,//GPIOB,
  NULL,//GPIOB_SPI1_CS,
  SPI_CR1_BR_2 | SPI_CR1_BR_1  |SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA
//  SPI_CR1_BR_2 |SPI_CR1_BR_1  | SPI_CR1_CPOL  
};
static const SPIConfig ls_spicfg = {
  NULL,
  GPIOB,
  9,
  SPI_CR1_BR_2 | SPI_CR1_BR_1
};

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL}, 
    {EXT_CH_MODE_DISABLED, NULL}, 
    {EXT_CH_MODE_DISABLED, NULL}, 
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE | EXT_MODE_GPIOB, ad7124RdyHandler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
  }
};


int ad7124RegRead(unsigned char regAddr, unsigned char *reg, unsigned char len)
{
  unsigned char ucTx[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  unsigned char ucRx[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  unsigned char i;
  
  spiAcquireBus(spiDev);
  spiStart(spiDev,&hs_spicfg);
  //spiSelect(spiDev);
  SPI_SELECT();
  ucTx[0] = REG_READ_MASK | regAddr;
  spiExchange(spiDev,len+1,ucTx,ucRx);
  //spiUnselect(spiDev);
  SPI_DESELECT();
  spiReleaseBus(spiDev);
  memcpy(reg,&ucRx[1],len);

  return 1;
}

int ad7124RegWrite(unsigned char regAddr, unsigned char *reg, unsigned char len)
{
  unsigned char ucTx[16]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
  unsigned char ucRx[8];
  unsigned char i;
  
  spiAcquireBus(spiDev);
  spiStart(spiDev,&hs_spicfg);
  //spiSelect(spiDev);
  SPI_SELECT();
  ucTx[0] = regAddr;
  memcpy(&ucTx[1],reg,len);
  spiExchange(spiDev,len+1,ucTx,ucRx);
  //spiUnselect(spiDev);
  SPI_DESELECT();
  spiReleaseBus(spiDev);
  
#ifdef REG_READBACK
  ad7124RegRead(regAddr,reg,len);
#endif
  
  return 0;
}

int ad7124SetVoltageBias(uint8_t pin)
{
  if(pin < 16){
    vBiasEn |= (1 << pin);
  }
  uint8_t tx[2];
  tx[0] = vBiasEn >> 8;
  tx[1] = vBiasEn;
  ad7124RegWrite(REG_VBIAS_ADDR,tx,2);
  return 0;
}

int ad7124ClearVoltageBias(uint8_t channel)
{
  if(channel < 16){
    vBiasEn &= ~(1 << channel);
  }
  uint8_t tx[2];
  tx[0] = vBiasEn >> 8;
  tx[1] = vBiasEn;
  ad7124RegWrite(REG_VBIAS_ADDR,tx,2);
  return 0;
}

int ad7124SetIout0(uint8_t ch0, uint8_t ch1, uint8_t cur0, uint8_t cur1)
{
  uint8_t tx[3] = {0,0,0};
  if(cur0 < 0xf){
    tx[1] = IO_IOUT0_RANG(cur0);
    tx[2] = IO_IOUT0_CH(ch0);
  }
  
  if(cur1 < 0xf){
    tx[1] |= IO_IOUT1_RANG(cur1);
    tx[2] |= IO_IOUT1_CH(ch1);
  }
  
  ad7124RegWrite(0x3,tx,3);
  return 0;
}

void ad7124ChannelSetup(uint8_t ch)
{
  uint8_t reg[2] = {0,0};
  uint8_t inp, inn;
  uint8_t inType;
  sysGetAnaInputPair(ch,&inp,&inn);
  inType = sysGetAnalogInType(ch);
  
  if(sysGetChannelEnable(ch)){
    reg[0] |= 0x80;
  }
  
  reg[0] |= CHANNEL_SETUP(inType);
  reg[0] |= (inp & 0x1f) >> 3;
  reg[1] = (inp & 0x1f) << 5 | inn;
  
  ad7124RegWrite(ADC_REG_CHANNEL(ch),reg,2);
  ad7124RegRead(ADC_REG_CHANNEL(ch),reg,2);
  
  uint8_t type = inType & 0xf0;
  ad7124ClearVoltageBias(inp);
  ad7124SetIout0(inp,inn,IOUT_OFF,IOUT_OFF);
  switch(type){
  case AI_IN_TC:
    ad7124SetVoltageBias(inp);
    break;
  case AI_IN_RTD:
    ad7124SetIout0(inp,inn,IOUT_100,IOUT_100);
    break;
  }
  
}

void ad7124InputConfig(uint8_t cfg)
{
  uint8_t reg[2] = {0,0};
  sysGetSetParam(cfg, reg);
  ad7124RegWrite(ADC_REG_CONFIG(cfg),reg,2);  
  ad7124RegRead(ADC_REG_CONFIG(cfg),reg,2);
}

void ad7124FilterConfig(uint8_t flt)
{
  uint8_t reg[3] = {0,0,0};
  sysGetFilterParam(flt, reg);
  ad7124RegWrite(ADC_REG_FILTER(flt),reg,3);  
  ad7124RegRead(ADC_REG_FILTER(flt),reg,3);
}

int ad7124SetChannelConfig(uint8_t ch, uint8_t config)
{
//  channelSetup[ch].s &= ~CHANNEL_SETUP_MASK;
//  channelSetup[ch].s  |= CHANNEL_SETUP(config);
//  ad7124RegWrite(ADC_REG_CHANNEL(ch), channelSetup[ch].b, 2);
//  uint8_t cfg = chSet[ch].b[0]& ~ADC_CH_SETUP_MASK;
//  cfg |= config;
//  chSet[ch].b[0] = cfg;
//  ad7124RegWrite(chSet[ch].regAddr, chSet[ch].b, chSet[ch].sz);
  
  return 0;
}

int halAd7124EnableChannel(uint8_t ch)
{
//  channelSetup[ch].s |= CHANNEL_ENABLE;  
//  ad7124RegWrite(ADC_REG_CHANNEL(ch), channelSetup[ch].b, 2);
  return 0;
}

int halAd7124DisableChannel(uint8_t ch)
{
//  channelSetup[ch].s &= CHANNEL_MASK;  
//  ad7124RegWrite(ADC_REG_CHANNEL(ch), channelSetup[ch].b, 2);
  return 0;
}

int halAd7124StartContConv()
{
  uint8_t reg[2];
  reg[0] = ADC_CTRL_DOUTRDY(1) | ADC_CTRL_CONT(1) | ADC_CTRL_DSTA(1) | ADC_CTRL_CSBEN(1) | ADC_CTRL_REFEN(1);
  reg[1] = ADC_CTRL_POWER(ADC_LOW_POWER) | ADC_CTRL_MODE(ADC_MODE_CONT) | ADC_CTRL_CLKSEL(ADC_CLK_INT_OUT_DIS);
  ad7124RegWrite(ADC_REG_CTRL,reg,2);
}

//int halAd7124StartConversionCont()
//{
//  adc_control.u.data_status = 1;
//  adc_control.u.power_mode = 3;
//  adc_control.u.mode = 0;
//  adc_control.u.cont_read = 1;
//  ad7124RegWrite(adc_control.regAddr, adc_control.b,2);
//  return 1;
//}

int halAd7124StartSingleConv()
{
  uint8_t reg[3];
  reg[0] = ADC_REG_CTRL;
  reg[1] = ADC_CTRL_DOUTRDY(1) | ADC_CTRL_CONT(0) | ADC_CTRL_DSTA(1) | ADC_CTRL_CSBEN(1) | ADC_CTRL_REFEN(1);
  reg[2] = ADC_CTRL_POWER(ADC_LOW_POWER) | ADC_CTRL_MODE(ADC_MODE_SINGLE) | ADC_CTRL_CLKSEL(ADC_CLK_INT_OUT_DIS);
  
  chSysLock();
  spiAcquireBus(spiDev);
  spiStart(spiDev,&hs_spicfg);
  SPI_SELECT();
  spiSend(spiDev,3,reg);
  spiReleaseBus(spiDev);
  chSysUnlock();
  
  return 1;
}

int halAd7124StartConversionSingle()
{
  uint8_t tx[8];
  uint8_t rx[8];

//  adc_control.u.data_status = 1;
//  adc_control.u.power_mode = 3;
//  adc_control.u.mode = 1;
//  adc_control.u.cont_read = 0;
//  adc_control.u.ref_en = 1;

//  tx[0] = adc_control.regAddr;
//  memcpy(&tx[1],adc_control.b,2);
  
  chSysLock();
  spiAcquireBus(spiDev);
  spiStart(spiDev,&hs_spicfg);
  //spiSelect(spiDev);
  SPI_SELECT();
  spiSend(spiDev,3,tx);
  spiReleaseBus(spiDev);
  chSysUnlock();
  /*
  chThdSleepMilliseconds(5);
  tx[0] = 0x42;
  spiSend(spiDev,1,tx);
  spiReceive(spiDev,4,rx);
  spiUnselect(spiDev);
  spiReleaseBus(spiDev);
  */

//  ad7124RegWrite(adc_control.regAddr, adc_control.b,2);
  return 1;
}

int halAd7124StopConversion()
{
  uint8_t reg[2];
  reg[0] = 0;
  reg[1] = ADC_CTRL_POWER(ADC_LOW_POWER) | ADC_CTRL_MODE(ADC_MODE_STBY) | ADC_CTRL_CLKSEL(ADC_CLK_INT_OUT_DIS);
  ad7124RegWrite(ADC_REG_CTRL, reg,2);
//  adc_control.u.data_status = 1;
//  adc_control.u.power_mode = 3;
//  adc_control.u.mode = 2;
//  adc_control.u.cont_read = 0;
//  adc_control.u.ref_en = 1;
//  adc_control.u.csb_en = 0;
//  adc_control.u.drdy_del = 0;
//  ad7124RegWrite(adc_control.regAddr, adc_control.b,2);
  return 1;  
}


uint8_t tch;
int halAd7124DataRead()
{
  unsigned char tx[8],rx[8];
  //ad7124RegRead(0x2,ucRx,4);
  // last byte contains the adc_status
  // drdy received, send 0x42 to request data
  tx[0] = 0x42;
  spiAcquireBus(spiDev);
  spiStart(spiDev,&hs_spicfg);
  spiSend(spiDev,1,tx);
  spiReceive(spiDev,4,rx);
  spiReleaseBus(spiDev);
  unsigned char ch = rx[3] & 0xf;
  long v = 0;
  v = (rx[0] << 16) | (rx[1] << 8) | (rx[2]);
  chrrentCh = ch;
  if((ch >=0) && (ch <nofChannel)){
    if(ch == tch){
      tch++;
    }
    sysSetAnaInData(ch,v);
//    chSet[ch].data = v;
//    chSet[ch].scaleV = fVolts0 * chSet[ch].data;
//    if(ch == 0){
//      chSet[ch].scaleV = (float)(chSet[ch].data*2000./268435456.);
//      //chSet[ch].scaleV = chSet[ch].scaleV*25000;
//    }
  }
  // todo: post data to buffer
  //spiReleaseBus(spiDev);
  /*
  // enable interrupt
  chSysLockFromISR();
  extChannelEnableI(&EXTD1,2);
  chSysUnlockFromISR();
  */
  return 0;
}

// todo: implement ext. interrupt for drdy signal
// & thread to read data

void ad7124RdyHandler(EXTDriver *extp, expchannel_t channel)
{
  chSysLockFromISR();
  extChannelDisableI(&EXTD1,4);
  chEvtSignalI(thdAD7124,EV_ADC_DATA_READY);
  chSysUnlockFromISR();
}




static THD_WORKING_AREA(waAD7124,1024);
static THD_FUNCTION(thAD7124 ,p)
{
  uint8_t i;
  bool bContinueRead = false;
  bool bSampling = false;
  uint8_t releaseCounter = 0;
//  EXTChannelConfig *oldcp;
  EXTConfig *cfg;
  spiDev = (SPIDriver*)p;
  spiStart(spiDev,&hs_spicfg);
  SPI_DESELECT();
  //spiUnselect(spiDev);

    chThdSleepMilliseconds(10);

  halAD7124Reset();
    chThdSleepMilliseconds(10);
  halAd7124StopConversion();

  extStart(&EXTD1,&extcfg);
  
  nofChannel = 8;
  uint8_t rx[8];

 
  for(i=0;i<9;i++){
    ad7124ChannelSetup(i);
  }
  for(i=0;i<8;i++){
    ad7124InputConfig(i);
  }
  for(i=0;i<8;i++){
    ad7124FilterConfig(i);
  }
//    ad7124RegWrite(chSet[i].regAddr, chSet[i].b, chSet[i].sz);
//    chThdSleepMilliseconds(1);
//    ad7124RegWrite(chCfg[i].regAddr, chCfg[i].b, chCfg[i].sz);
//    chThdSleepMilliseconds(1);
//    ad7124RegWrite(chFlt[i].regAddr, chFlt[i].b, chFlt[i].sz);
//    chThdSleepMilliseconds(1);
//  }
  
  //ad7124SetVoltageBias(0);
  ad7124SetIout0(2,3,IOUT_OFF,IOUT_OFF);
  ad7124SetIout0(0,1,IOUT_1000,IOUT_1000);
  
  //uint8_t rx[8];
  ad7124RegRead(0x3,rx,3);
  
  while(true){
    eventmask_t evt = chEvtWaitAnyTimeout(ALL_EVENTS,10);
    if(bSampling) 
      releaseCounter++;
    
    if(releaseCounter > 200){
      releaseCounter = 0;
      SPI_DESELECT();
      bSampling = false;
      extChannelDisable(&EXTD1,4);
    }
    //chEvtGetAndClearEvents(ALL_EVENTS);
    if(evt & EV_ADC_DATA_READY){
      // disable interrupt function
      extChannelDisable(&EXTD1,4);
      // read data
      palSetGroupMode(GPIOB,PAL_PORT_BIT(4),0,PAL_STM32_ALTERNATE(5) | PAL_STM32_MODE_ALTERNATE);
      halAd7124DataRead();
      chCount++;
      if(chCount == nofChannel){
        if(bContinueRead){
          // enable interrupt
          palSetGroupMode(GPIOB,PAL_PORT_BIT(4),0,PAL_STM32_ALTERNATE(0));
          extChannelEnable(&EXTD1,4);
        }else{
          // release bus
          SPI_DESELECT();
          bSampling = false;
        }
      }else{
        palSetGroupMode(GPIOB,PAL_PORT_BIT(4),0,PAL_STM32_ALTERNATE(0));
        extChannelEnable(&EXTD1,4);
      }
    }
    if(evt & EV_ADC_DRDY){
      //halAd7124DataRead();
    }
    if(evt & EV_ADC_START_SINGLE){
      if(!bSampling){
        bSampling = true;
        releaseCounter = 0;
        bContinueRead = false;
        chCount = 0;
        halAd7124StartSingleConv();
        palSetGroupMode(GPIOB,PAL_PORT_BIT(4),0,PAL_STM32_ALTERNATE(0));
        extChannelEnable(&EXTD1,4);
      }
    }
    if(evt & EV_ADC_START_CONT){
      bContinueRead = true;
      halAd7124StartContConv();
    }
    if(evt & EV_ADC_STOP){
      halAd7124StopConversion();
    }
    if(evt & EV_ADC_START_CALI){
    }
    if(evt & EV_ADC_DONE_CALI){
    }
    if(evt & EV_ADC_CONFIG){
      
    }
    
    
  }
}

int halAD7124Reset()
{
  uint8_t reg[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  ad7124RegWrite(0xff,reg,8);
  return 0;
}

int halAD7124ScanRegs()
{
  uint8_t rx[8];
  uint8_t i;
    // read ID register
  ad7124RegRead(0x0,rx,2);
  ad7124RegRead(0x1,rx,2);
  ad7124RegRead(0x2,rx,2);
  ad7124RegRead(0x5,rx,1);
  ad7124RegRead(0x6,rx,3);
  ad7124RegRead(0x7,rx,3);

  for(i=0;i<8;i++){
    ad7124RegRead(i+0x9,rx,2);
    ad7124RegRead(i+0x19,rx,2);
  }
  return 0;
}

uint8_t halAD7124ReadStatus()
{
  /*
  unsigned char ucTx[]= {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
  unsigned char ucRx[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  unsigned char i;
  ucTx[0] = REG_READ_MASK;
  
  spiAcquireBus(spiDev);
  spiStart(spiDev,&hs_spicfg);
  spiSelect(spiDev);
  spiExchange(spiDev,2,ucTx,ucRx);
  spiUnselect(spiDev);
  spiReleaseBus(spiDev);
  return (ucRx[1]);
  */
  uint8_t sta;
  ad7124RegRead(0x0,&sta,1);
  return sta;
}


int halAd7124Init(SPIDriver *spi)
{
  int i;
  uint8_t sta;
  uint8_t rx[8],tx[8];
  
  spiDev = spi;
  /*
  spiUnselect(spiDev);
  spiSelect(spiDev);
  spiUnselect(spiDev);
  */

 
   
  // create working thread
  thdAD7124 = chThdCreateStatic(waAD7124,sizeof(waAD7124),NORMALPRIO,thAD7124,(SPIDriver*)spi);
  
  return 1;
}

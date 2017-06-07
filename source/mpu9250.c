#include "ch.h"
#include "hal.h"
#include "mpu9250.h"

// MPU9250 connect to SPI2 of STM32F2

#define SPI_SELECT() palClearPad(GPIOB,12)
#define SPI_DESELECT() palSetPad(GPIOB,12)

static SPIDriver *spiDev;
static const SPIConfig hs_spicfg = {
  NULL,
  GPIOB,
  GPIOB_ACC_CS_O,
  SPI_CR1_BR_2 | SPI_CR1_BR_1  |SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA
};

// interrupt routine prototype
void mpu9250_int_handler(EXTDriver *extp, expchannel_t channel);

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL}, 
    {EXT_CH_MODE_DISABLED, NULL}, 
    {EXT_CH_MODE_DISABLED, NULL}, 
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE | EXT_MODE_GPIOC, mpu9250_int_handler},
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

void mpu9250_int_handler(EXTDriver *extp, expchannel_t channel)
{
  
}

int hal9250_RegRead(unsigned char regAddr, unsigned char *reg, unsigned char len)
{
  unsigned char ucTx[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  unsigned char ucRx[]= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  unsigned char i;
  
  spiAcquireBus(spiDev);
  SPI_SELECT();
//  ucTx[0] = REG_READ_MASK | regAddr;
  spiExchange(spiDev,len+1,ucTx,ucRx);
  SPI_DESELECT();
  spiReleaseBus(spiDev);
  memcpy(reg,&ucRx[1],len);

  return 1;
}

int hal9250_RegWrite(unsigned char regAddr, unsigned char *reg, unsigned char len)
{
  unsigned char ucTx[16]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
  unsigned char ucRx[8];
  unsigned char i;
  
  spiAcquireBus(spiDev);
  SPI_SELECT();
  ucTx[0] = regAddr;
  memcpy(&ucTx[1],reg,len);
  spiExchange(spiDev,len+1,ucTx,ucRx);
  SPI_DESELECT();
  spiReleaseBus(spiDev);
  
  return 0;
}

void mpu9250EnableFIFO(uint8_t mask)
{
  
  
}

uint16_t mpu9250FifoLength(void)
{
  uint16_t len = 0;
  hal9250_RegRead(REG_FIFO_CNTH,(uint8_t*)&len,2);
  return len;
}

void mpu9250_EnableFifo(uint8_t mask)
{
  uint16_t reg = 0x78;
  hal9250_RegWrite(REG_FIFO_EN,(uint8_t*)&reg,1);
  reg = 0x0;
  hal9250_RegRead(REG_FIFO_EN,(uint8_t*)&reg,1);
}
int16_t mpu8250_ReadFifo(uint8_t *dptr, uint16_t len)
{
  // check fifo size
  uint16_t fifoSize = mpu9250FifoLength();
  if(fifoSize < len) return -1;
  hal9250_ReadReg(REG_FIFO,dptr,len);
  return len;
}

// power management 2
void mpu9250_AxisEnable(uint8_t axisEn)
{
  uint8_t reg;
  switch(axisEn){
  case 0x0:
    reg = 0x0;
    break;
  case 0x1:
    reg = 0x38;
    break;
  case 0x2:
    reg = 0x7;
    break;
  }
  
  hal9250_RegWrite(REG_PWR_MGMT_2,&reg,1);
}

void mpu9250_EnableInterrupt(uint8_t mask)
{
  
  
}

void mpu9250_SetGyroRange(uint8_t range)
{
  uint16_t reg = range << 3;
  hal9250_RegWrite(REG_GYRO_CFG,(uint8_t*)&reg,1);
  reg = 0x0;
  hal9250_RegRead(REG_GYRO_CFG,(uint8_t*)&reg,1);
  
}

void mpu9250_SetAccRange(uint8_t range)
{
  uint16_t reg = range << 3;
  hal9250_RegWrite(REG_ACCEL_CFG,(uint8_t*)&reg,1);
  reg = 0x0;
  hal9250_RegRead(REG_ACCEL_CFG,(uint8_t*)&reg,1);
  
}

void mpu9250_SetAccRate(uint8_t cfg)
{
  uint16_t reg = cfg;
  hal9250_RegWrite(REG_ACCEL_CFG2,(uint8_t*)&reg,1);
  reg = 0x0;
  hal9250_RegRead(REG_ACCEL_CFG2,(uint8_t*)&reg,1);
  
}

void mpu9250_SetSampleRate(uint16_t rate)
{
  uint8_t reg = 1000/rate - 1;
  hal9250_RegWrite(REG_ACCEL_CFG2,&reg,1);
  reg = 0x0;
  hal9250_RegRead(REG_ACCEL_CFG2,&reg,1);  
  
}





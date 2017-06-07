#include "ch.h"
#include "hal.h"
#include "hal_ad5593r.h"

I2CDriver *i2cBus;
uint16_t ADCResult[16];
uint16_t DACData[16];

void ad5593rInit(I2CDriver *bus)
{
  uint8_t tx[8],rx[8];
  i2cBus = bus;
  msg_t res;
  // setup AD5593R for A/D converter
  // two AD5593R in this system
  ad5593rSetToADC(AD5593R_CHIP_ADDR_0,0xff);
  ad5593rSetToADC(AD5593R_CHIP_ADDR_1,0xff);
  
  // set ADC input range to 2xVref
  tx[0] = CFG_GPCR;
  tx[1] = 0x3;
  tx[2] = 0x20;
  res = i2cMasterTransmitTimeout(i2cBus,
                           AD5593R_CHIP_ADDR_0,
                           tx,
                           3,
                           rx,
                           0,
                           100);
  res = i2cMasterTransmitTimeout(i2cBus,
                           AD5593R_CHIP_ADDR_1,
                           tx,
                           3,
                           rx,
                           0,
                           100);

  tx[0] = CFG_PWR;
  tx[1] = 0x2;
  tx[2] = 0x0;
  res = i2cMasterTransmitTimeout(i2cBus,
                           AD5593R_CHIP_ADDR_0,
                           tx,
                           3,
                           rx,
                           0,
                           100);
  res = i2cMasterTransmitTimeout(i2cBus,
                           AD5593R_CHIP_ADDR_1,
                           tx,
                           3,
                           rx,
                           0,
                           100);
}
void ad5593rSetToDAC(uint8_t sla, uint8_t mask)
{
  msg_t res;
  uint8_t tx[8],rx[8];
  
  tx[0] = CFG_DAC_PIN;
  tx[1] = 0x00;
  tx[2] = 0xff;
  
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla,
                           tx,
                           3,
                           rx,
                           0,
                           100);
  
  
  
  tx[0] = CFG_ADC_SEQ;
  tx[1] = 0x02;
  tx[2] = 0xff;
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla,
                           tx,
                           3,
                           rx,
                           0,
                           100);
    
}

void ad5593rSetToADC(uint8_t sla, uint8_t mask)
{
  static i2cflags_t errors ;
  msg_t res;
  uint8_t tx[8],rx[8];
  tx[0] = CFG_ADC_PIN;
  tx[1] = 0x00;
  tx[2] = 0xff;
  
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla,
                           tx,
                           3,
                           rx,
                           0,
                           MS2ST(100));
  if(res == MSG_RESET){
    errors = i2cGetErrors(&I2CD1);
  }
  tx[0] = CFG_ADC_SEQ;
  tx[1] = 0x02;
  tx[2] = 0xff;
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla,
                           tx,
                           3,
                           rx,
                           0,
                           MS2ST(100));
  
}

void ad5593rReadADC(uint8_t sla, uint8_t nofch, uint16_t *data)
{
  msg_t res;
  uint8_t tx[4], rx[16];
  tx[0] = REG_ADCRD;
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla,
                           tx,
                           1,
                           rx,
                           0,
                           100);
  
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla | 0x1,
                           tx,
                           1,
                           rx,
                           16,
                           100);
  
  memcpy((char*)data,rx,16);
}

void ad5593rADCScan()
{
  msg_t res;
  uint8_t i;
  uint8_t tx[4], rx[32];
  tx[0] = REG_ADCRD;
  //ad5593rSetToADC(AD5593R_CHIP_ADDR_0,0xff);
  
  
  res = i2cMasterTransmitTimeout(&I2CD1,
                           AD5593R_CHIP_ADDR_0,
                           tx,
                           1,
                           rx,
                           0,
                           100);
  
  tx[0] = REG_ADCRD;
  
  res = i2cMasterTransmitTimeout(&I2CD1,
                           AD5593R_CHIP_ADDR_0,
                           tx,
                           1,
                           rx,
                           16,
                           100);
  
  //memcpy((char*)&ADCResult[0],rx,16);
  ADCResult[7] = rx[0] << 8 | rx[1];
  ADCResult[6] = rx[2] << 8 | rx[3];
  ADCResult[5] = rx[4] << 8 | rx[5];
  ADCResult[4] = rx[6] << 8 | rx[7];
  ADCResult[3] = rx[8] << 8 | rx[9];
  ADCResult[2] = rx[10] << 8 | rx[11];
  ADCResult[1] = rx[12] << 8 | rx[13];
  ADCResult[0] = rx[14] << 8 | rx[15];

  //ad5593rSetToADC(AD5593R_CHIP_ADDR_1,0xff);
/*
  res = i2cMasterTransmitTimeout(&I2CD1,
                           AD5593R_CHIP_ADDR_1,
                           tx,
                           1,
                           rx,
                           0,
                           100);
  */
  res = i2cMasterTransmitTimeout(&I2CD1,
                           AD5593R_CHIP_ADDR_1,
                           tx,
                           1,
                           rx,
                           16,
                           100);
  
  //memcpy((char*)&ADCResult[8],rx,16);
  ADCResult[7+8] = rx[0] << 8 | rx[1];
  ADCResult[6+8] = rx[2] << 8 | rx[3];
  ADCResult[5+8] = rx[4] << 8 | rx[5];
  ADCResult[4+8] = rx[6] << 8 | rx[7];
  ADCResult[3+8] = rx[8] << 8 | rx[9];
  ADCResult[2+8] = rx[10] << 8 | rx[11];
  ADCResult[1+8] = rx[12] << 8 | rx[13];
  ADCResult[0+8] = rx[14] << 8 | rx[15];
  
  for(i=0;i<16;i++){
    ADCResult[i] &= 0xfff;
  }
  
  i = 5;
}

uint16_t ad5593rReadChRaw(uint8_t ch)
{
  return ADCResult[ch];
}
float ad5593rReadChFloat(uint8_t ch)
{
  return ((float)ADCResult[ch]*AD5593R_LSB);
}

void ad5593rWriteDAC(uint8_t sla, uint8_t nofch, uint16_t *data)
{
  msg_t res;
  uint8_t tx[4], rx[16];
  tx[0] = REG_ADCRD;
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla,
                           tx,
                           1,
                           rx,
                           0,
                           100);
  
  res = i2cMasterTransmitTimeout(&I2CD1,
                           sla | 0x1,
                           tx,
                           1,
                           rx,
                           16,
                           100);
  
  memcpy((char*)data,rx,16);  
}
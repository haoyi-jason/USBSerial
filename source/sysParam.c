#include "ch.h"
#include "hal.h"
#include "sysParam.h"
#include "i2c_eeprom.h"
#include "hal_ad7124.h"
#include "thermocouple.h"

extern thread_t *thdAD7124;
static sys_param_t sysParam;
static analog_input_t anaIn[] =
{
  {1,AI_IN_VOLT,0,2,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,1,3,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,4,6,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,5,7,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,8,10,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,9,11,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,12,14,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,13,15,10,-10,PGA_X1,0,(INT_VREF_V*VOLT_RATIO)/(1<<23),CURRENT_SHUNT,1.0},
  {1,AI_IN_VOLT,0x10,0x10,10,-10,PGA_X1,0,1.0,1.0,1.0},
};
static analog_input_type_t anaType[] = 
{
  {1,0,0,0,0,0,REF_INT_2_5,PGA_X32},     // TC
  {1,0,0,0,0,0,REF_EXT1,PGA_X8},     // RTD
  {1,0,0,0,0,0,REF_INT_2_5,PGA_X1},     // Volt
  {1,0,0,0,0,0,REF_INT_2_5,PGA_X1},     // current
  {0,0,0,0,0,0,REF_INT_2_5,PGA_X1},
  {0,0,0,0,0,0,REF_INT_2_5,PGA_X1},
  {0,0,0,0,0,0,REF_INT_2_5,PGA_X1},
  {0,0,0,0,0,0,REF_INT_2_5,PGA_X1},
};
static analog_filter_type_t filter[] = 
{
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
  {FLT_SINC4,1,0,0,10},
};

static digital_io_type_t digIO[4];

digital_io_type_t *sysGetDIO(uint8_t ch){
  return &digIO[ch];
}

void sysSetDioVal(uint8_t ch, uint32_t v)
{
  digIO[ch].val = v;
}

uint32_t sysGetDioVal(uint8_t ch)
{
  return digIO[ch].val;
}

void sysSetDioType(uint8_t ch, uint8_t type)
{
  digIO[ch].type = type;
}

uint8_t sysGetDioType(uint8_t ch)
{
  return digIO[ch].type;
}

void sysSetDioDir(uint8_t ch, uint8_t dir)
{
  digIO[ch].direction = dir;
}

uint8_t sysGetDioDir(uint8_t ch)
{
  return digIO[ch].direction;
}

uint8_t sysGetChannelEnable(uint8_t ch)
{
  return anaIn[ch].enable;
}

void sysSetChannelEnable(uint8_t ch, uint8_t state)
{
  anaIn[ch].enable = state;
}

void sysGetSetParam(uint8_t no, uint8_t *cfg)
{
  cfg[0] = cfg[1] = 0;
  cfg[0] |= (anaType[no].bipolar << 3);
  cfg[0] |= (anaType[no].burnout << 1);
  cfg[0] |= anaType[no].refbp;
  
  cfg[1] |= (anaType[no].refbn << 7);
  cfg[1] |= (anaType[no].ainbp << 6);
  cfg[1] |= (anaType[no].ainbn << 5);
  cfg[1] |= (anaType[no].refsel << 3);
  cfg[1] |= (anaType[no].pga << 0);
}

void sysGetFilterParam(uint8_t no, uint8_t *cfg)
{
  cfg[0] = (filter[no].filter << 5) | (filter[no].rej60<<4) | (filter[no].postFilter<<1) | filter[no].singleCycle;
  cfg[1] = (filter[no].fs & 0x7ff) >> 8;
  cfg[2] = filter[no].fs;
}

int8_t sysSetAnalogInType(uint8_t ch, uint8_t type)
{
  if(type != anaIn[ch].inputType){
    anaIn[ch].inputType = type;
    switch(type){
    case AI_IN_TC:
      anaIn[ch].gain = PGA_X32;
      anaIn[ch].scalingFactor = ((float)(INT_VREF_V))/(1<<PGA_X32);
      break;
    case AI_IN_RTD:
      anaIn[ch].scalingFactor = 1.0;
      anaIn[ch].gain = PGA_X8;
      break;
    case AI_IN_VOLT:    // use internal 2.5Vref
      anaIn[ch].gain = PGA_X1;
      anaIn[ch].scalingFactor = INT_VREF_V*VOLT_RATIO/(1<<24);
      break;
    case AI_IN_AMP:
      anaIn[ch].scalingFactor = INT_VREF_V*VOLT_RATIO/anaIn[ch].shuntResistor/(1<<24);
      break;
    }
    // notify 7124 thread to update
    chEvtSignal(thdAD7124,EV_ADC_CONFIG);
  }
  return 0;
}

int8_t sysSetAnalogSensorType(uint8_t ch, uint8_t type)
{
  anaIn[ch].sensorType = type;
  return 0;
}

uint8_t sysGetAnalogSensorType(uint8_t ch)
{
  return anaIn[ch].sensorType;
}

int8_t sysGetAnalogInType(uint8_t ch)
{
  int8_t ret;
  ret = anaIn[ch].inputType;
  /*
  switch(anaIn[ch].inputType){
  case AI_IN_TC:
    ret = CHS_TC;
    break;
  case AI_IN_RTD:
    ret = CHS_RTD;
    break;
  case AI_IN_VOLT:
    ret = CHS_VOLT;
    break;
  case AI_IN_AMP:
    ret = CHS_CURRENT;
    break;
  }
  */
  return ret;
}

void sysGetAnaInputPair(uint8_t ch,uint8_t *p, uint8_t *n)
{
  *p = anaIn[ch].inp;
  *n = anaIn[ch].inn;
}

void sysSetAnaInputPair(uint8_t ch, uint8_t p, uint8_t n)
{
  anaIn[ch].inp = p;
  anaIn[ch].inn = n;
}

int8_t sysGetAnaInData(uint8_t ch, uint16_t *val)
{
  int8_t ret;
  switch(anaIn[ch].inputType){
  case AI_IN_TC:
  case AI_IN_RTD:
    *val = (uint16_t)(anaIn[ch].scaledValue*100);
    break;
  case AI_IN_VOLT:
    *val = (uint16_t)(anaIn[ch].scaledValue*1000);
    break;
  case AI_IN_AMP:
    *val = (uint16_t)(anaIn[ch].scaledValue*1000);
    break;
  }
  return ret;
    
}

int8_t sysSetAnaInData(uint8_t ch, uint32_t data)
{
  double v;
  switch(anaIn[ch].inputType){
  case AI_IN_TC: // GAIN = 32
    anaIn[ch].adc_value = data;
    v = data * anaIn[ch].scalingFactor*1000;
    anaIn[ch].scaledValue = getTempFromV(anaIn[ch].sensorType,v);
    break;
  case AI_IN_RTD: // GAIN=8, Iout = ?, shunt resistor = ?
    anaIn[ch].adc_value = data;
    v = data * anaIn[ch].scalingFactor*1000;
    anaIn[ch].scaledValue = getTempFromV(anaIn[ch].sensorType,v);
    break;
  case AI_IN_VOLT:// GAIN=1,voltage divider = 1.02/ (1.02+8.87), 
    anaIn[ch].adc_value = data;
    anaIn[ch].scaledValue = (int32_t)(data - BIPOLAR_OFFSET) * anaIn[ch].scalingFactor;
    break;
  case AI_IN_AMP: // GAIN=1, shunt resistor = ?
    anaIn[ch].adc_value = data;
    anaIn[ch].scaledValue = data * anaIn[ch].scalingFactor;
    break;
  }
  return 0;
}


int8_t sysSetDigInType(uint8_t type)
{
  if(type != sysParam.dinType){
    sysParam.dinType = type;
    
    // notify system to update
  }
  return 0;
}

int8_t sysGetDigInType()
{ 
  return sysParam.dinType;
}

int8_t sysSetDigOutType(uint8_t type)
{
  if(type != sysParam.doutType){
    sysParam.doutType = type;
    // notify system update
  }
  return 0;
}

int8_t sysGetDigOutType()
{
  return sysParam.doutType;
}

int8_t sysSaveParam()
{
    eepromWrite(SYSPARAM_OFFSET,sizeof(sysParam),(uint8_t*)&sysParam);
    eepromWrite(STORAGE_OFFSET,sizeof(anaIn),(uint8_t*)&anaIn);
    return 1;
}

void sysParamInit()
{
  eepromInit();
  eepromRead(SYSPARAM_OFFSET,sizeof(sysParam),(uint8_t*)&sysParam);
  
  if(sysParam.flag != HEADER_FLAG){
    sysParam.flag = HEADER_FLAG;
    
    
    uint8_t i;
    for(i=0;i<sizeof(anaIn)/sizeof(analog_input_t);i++){
      anaIn[i].enable = 1;
      anaIn[i].inp = i*2;
      anaIn[i].inn = i*2 + 1;
      anaIn[i].inputType = AI_IN_VOLT;
//      anaIn[i].inputType = AI_IN_TC;
      anaIn[i].uLimit = 10.0;
      anaIn[i].lLimit = -10.0;
      anaIn[i].scalingFactor = INT_VREF_V*VOLT_RATIO/(1<<23);
      anaIn[i].gain = PGA_X1;
      anaIn[i].shuntResistor = 250;
    }
    
    
    sysSaveParam();
    //eepromWrite(SYSPARAM_OFFSET,sizeof(sysParam),(uint8_t*)&sysParam);
  }else{
    eepromRead(STORAGE_OFFSET,sizeof(anaIn),(uint8_t*)&anaIn);
  }
}
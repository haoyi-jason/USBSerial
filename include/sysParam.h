#ifndef _SYSPARAM_
#define _SYSPARAM_

#define HEADER_FLAG     0xaa
#define SYSPARAM_OFFSET 0x00
#define STORAGE_OFFSET  0x80
#define CHPARAM_OFFSET  0xa0

#define AI_IN_TC        0x10
#define AI_IN_RTD       0x20
#define AI_IN_VOLT      0x30
#define AI_IN_AMP       0x40

#define DI_IN_IO        0x10
#define DI_IN_CNTR      0x20
#define DI_IN_CAPTURE   0x30
#define DI_IN_FREQ      0x40

#define DI_OUT_IO       0x10
#define DI_OUT_NPULSE   0x20
#define DI_OUT_PWM      0x30
#define DI_OUT_FREQ     0x40

#define VOLT_DIV_H      8.87
#define VOLT_DIV_L      1.02
#define VOLT_RATIO      (VOLT_DIV_H+VOLT_DIV_L)/VOLT_DIV_L
#define INT_VREF_V      2.5
#define CURRENT_SHUNT   50
#define BIPOLAR_OFFSET  (1 << 23)

typedef struct sys_param_s{
  uint8_t flag;
  uint8_t ainType;
  uint8_t dinType;
  uint8_t doutType;
  uint32_t analogChannelMask;
  uint32_t digInChannelMask;
  uint32_t digOutChannelMask;
}sys_param_t;

typedef struct{
  uint8_t enable;
  uint8_t inputType;
  uint8_t sensorType;
  uint8_t inp;
  uint8_t inn;
  int8_t uLimit;
  int8_t lLimit;
  int16_t gain;
  long adc_value;
  double scalingFactor;
  double shuntResistor;
  double scaledValue;
}analog_input_t;

typedef struct{
  uint8_t bipolar;
  uint8_t burnout;
  uint8_t refbp;
  uint8_t refbn;
  uint8_t ainbp;
  uint8_t ainbn;
  uint8_t refsel;
  uint8_t pga;
}analog_input_type_t;

typedef struct{
  uint8_t filter;
  uint8_t rej60;
  uint8_t postFilter;
  uint8_t singleCycle;
  uint16_t fs;
}analog_filter_type_t;


typedef struct{
  uint8_t direction;
  uint8_t type;
  uint32_t val;
  uint32_t fraction;
  uint32_t base;
  ioportid_t port;
  uint16_t pad;
}digital_io_type_t;

#define NOF_DIG_IO      4
enum dio_dir_e {DIO_INPUT,DIO_OUTPUT};
enum dir_type_e {DIO_IO,DIO_PWM,DIO_CNTR};



uint8_t sysGetChannelEnable(uint8_t ch);
void sysSetChannelEnable(uint8_t ch, uint8_t state);
void sysGetSetParam(uint8_t no, uint8_t *cfg);
void sysGetFilterParam(uint8_t no, uint8_t *cfg);
void sysGetAnaInputPair(uint8_t ch,uint8_t *p, uint8_t *n);
int8_t sysSetAnalogInType(uint8_t ch, uint8_t type);
int8_t sysGetAnaInData(uint8_t ch, uint16_t *val);
int8_t sysSetAnaInData(uint8_t ch, uint32_t data);
int8_t sysGetAnalogInType(uint8_t ch);
int8_t sysSetDigInType(uint8_t type);
int8_t sysGetDigInType();
int8_t sysSetDigOutType(uint8_t type);
int8_t sysGetDigOutType();
int8_t sysSaveParam();
void sysParamInit();


#endif
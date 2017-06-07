#ifndef _THERMOCOUPLE_
#define _THERMOCOUPLE_

enum sensor_type_e{
  TC_K,
  TC_J,
  PT_100,
  PT_1000
};


double getTempFromV(uint8_t type, double v);
void thermocoupleInit();

#endif
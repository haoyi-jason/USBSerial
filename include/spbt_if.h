#ifndef _SPBT_IF_
#define _SPBT_IF_
#include "ch.h"
#include "hal.h"

typedef struct{
  uint8_t autoReconnect;
  uint32_t baudrate;
  
};

#define EVENT_CMD_RESPONSED     EVENT_MASK(1)
#define EVENT_DATA_ARRIVED      EVENT_MASK(2)
#endif













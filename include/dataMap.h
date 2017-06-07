#ifndef _DATAMAP_H
#define _DATAMAP_H

#include "ch.h"

typedef struct {
  uint16_t inputType;
  uint16_t offset;
  uint16_t gain;
  uint32_t val;
  uint16_t unit;
  uint32_t lmtHigh;
  uint32_t lmtLow;
}_adc_map_t;

#endif
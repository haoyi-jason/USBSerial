#include "util.h"
#include "ch.h"
#include "hal.h"


_GPIO_Map digitIn[24];
_GPIO_Map digitOut[10];
_GPIO_Map test;

uint16_t readBoardId()
{
  uint16_t id = palReadPort(GPIOB);
  id >>= 8;
  id |= 0x03;
  return 255-id;
}

uint8_t digitalWrite(uint8_t nbr, uint8_t state)
{
  int r;
  if(state){
      //r=5;
    //palSetPad(digitOut[nbr].port,digitOut[nbr].pin);
    palSetPad(GPIOA,test.pin);
  }
  else{
      //palClearPad(digitOut[nbr].port,digitOut[nbr].pin);
  }
}
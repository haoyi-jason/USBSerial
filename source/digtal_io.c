#include "ch.h"
#include "hal.h"
#include "digital_io.h"
#include "sysParam.h"




void digital_io_init()
{
  uint8_t i;
  
  for(i=0;i<NOF_DIG_IO;i++){
    uint8_t dir;
    uint8_t type;
    if(sysGetDioDir(i) == DIO_INPUT){
      switch(sysGetDioType(i)){
      case DIO_IO:
        
        break;
      case DIO_PWM:
        break;
      case DIO_CNTR:
        break;
      }
    }else{
      
    }
    
  }
}
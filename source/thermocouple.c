#include "ch.h"
#include "hal.h"
#include "thermocouple.h"

#define TCK_MIN_MV      -5.730
#define TCK_MAX_MV      15.975
#define TCK_SEG_N       20
#define TCK_SEG_P       40
#define TCK_TEMP_SEG    10

const float K_tctableN[] = {
  0,            -0.392, -0.778, -1.156, -1.527, -1.889, -2.243, -2.587, 2.920,  -3.243,
  -3.554,       -3.852, -4.138, -4.411, -4.669, -4.913, -5.141, -5.354, -5.550, -5.730
};

const float K_tctableP[] = {
  0,            0.397,  0.798,  1.203,  1.612,  2.023,  2.436,  2.851,  3.267,  3.682,  
  4.096,        4.509,  4.920,  5.328,  5.735,  6.138,  6.540,  6.941,  7.340,  7.739,  
  8.138,        8.539,  8.940,  9.343,  9.747,  10.153, 10.561, 10.971, 11.382, 11.795,
  12.209,       12.624, 13.040, 13.457, 13.874, 14.293, 14.713, 15.133, 15.554, 15.975,
};

const float PT100_tableN[] = {
  100,  96.09,  92.16,  88.22,  84.27,  80.31,  76.33,  72.33,  68.33,  64.30,  60.26,
};

const float PT100_tableP[] = {
  100,  103.9,  107.79, 111.67, 115.54, 119.40, 123.24, 127.08, 130.90, 134.71, 138.51,            
};




static uint8_t sensorType;
static double coldT;
static double sensorT;

void tckFromMvToT(double mv, double *t)
{
  uint8_t i;
  if(mv == 0.0){
    *t = 0.;
  }else if(mv > 0.0){
    if(mv > TCK_MAX_MV) *t = 0.0;
    else{
      for(i=0;i<TCK_SEG_P;i++){
        if(mv < K_tctableP[i]){
          i--;
          break;
        }
      }
      *t = (mv - K_tctableP[i])*TCK_TEMP_SEG/(K_tctableP[i+1] - K_tctableP[i]) + i*TCK_TEMP_SEG;
    }
  }else{
    if(mv < TCK_MIN_MV) *t = 0.0;
    else{
      for(i=0;i<TCK_SEG_N;i++){
        if(mv < K_tctableN[i]){
          i--;
          break;
        }
      }
      *t = (mv - K_tctableP[i])*TCK_TEMP_SEG/(K_tctableP[i+1] - K_tctableP[i]) + i*TCK_TEMP_SEG;
    }
  }
  
}

//void updateSensorVolt(double volt)
double getTempFromV(uint8_t type, double v)
{
  double t = 0;
  switch(type){
  case TC_K:
    tckFromMvToT(v,&t);
    break;
  case TC_J:
    break;
  case PT_100:
    break;
  case PT_1000:
    break;
  }
  return t;
}

void updateColdT(double t)
{
  
}


void thermocoupleInit()
{
  
}
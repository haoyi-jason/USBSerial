#ifndef _MPU9250_
#define _MPU9250_

#include "ch.h"
#include "hal.h"

// register map
enum mpu9250_reg_e{
  REG_SELF_TEST_XA=0x0D,
  REG_SELF_TEST_YA,
  REG_SELF_TEST_ZA,
  REG_SELF_TEST_A,
  REG_SMPLRT_DIV = 0x19,
  REG_CONFIG,
  REG_GYRO_CFG,
  REG_ACCEL_CFG,
  REG_ACCEL_CFG2,
  REG_FIFO_EN=0x23,
  REG_INT_PIN_CFG=0x37,
  REG_INT_ENABLE,
  REG_INT_STATUS = 0x3A,
  REG_ACCEL_XH,
  REG_ACCEL_XL,
  REG_ACCEL_YH,
  REG_ACCEL_YL,
  REG_ACCEL_ZH,
  REG_ACCEL_ZL,
  REG_TEMP_H,
  REG_TEMP_L,
  REG_GYRO_XH,
  REG_GYRO_XL,
  REG_GYRO_YH,
  REG_GYRO_YL,
  REG_GYRO_ZH,
  REG_GYRO_ZL,
  REG_SIG_PATH_RST=0x68,
  REG_USER_CTRL = 0x6A,
  REG_PWR_MGMT_1,
  REG_PWR_MGMT_2,
  REG_FIFO_CNTH=0x72,
  REG_FIFO_CNTL,
  REG_FIFO,
  REG_WHOAMI
};

#define GYRO_FS_RANG(x) (x << 3)
enum gyro_rang_e{
  GYRO_250,
  GYRO_500,
  GYRO_1000,
  GYRO_2000
};

#define ACCEL_FS_RANG(x) (x << 3)
enum accel_range_e{
  ACCEL_2G,
  ACCEL_4G,
  ACCEL_8G,
  ACCEL_16G
};

#define SLV0_FIFO_EN    0x01
#define SLV1_FIFO_EN    0x02
#define SLV2_FIFO_EN    0x04
#define ACCEL_FIFO_EN   0x08
#define GYRO_Z_FIFO_EN  0x10
#define GYRO_Y_FIFO_EN  0x20
#define GYRO_X_FIFO_EN  0x40
#define TEMP_FIFO_EN    0x80

#define FIFO_OFLOW_EN   0x10
#define DATA_RDY_EN     0x01

// register 0x6A
#define SIG_COND_RST    0x01
#define FIFO_RST        0x04
#define FIFO_EN         0x40

// register 0x6B
#define DEV_SLEEP       0x40
#define DEV_RESET       0x80



#endif
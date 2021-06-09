#ifndef IMU_H__
#define IMU_H__
// Header file for IMU.c

#include <xc.h>

#define WHO_AM_I 0xf
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12

void imu_setup();
unsigned char imu_read();
#endif
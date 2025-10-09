#ifndef LMP_PROJECT_HARDWARE_IMU_COMMON_H_
#define LMP_PROJECT_HARDWARE_IMU_COMMON_H_

/**
 * @file common.h
 * @brief Common definitions and includes for IMU drivers (MPU6050 etc.).
 * @note Internal use only within hardware/imu modules.
*/


#include "imu/return_code.h"
#include "imu/mpu6050_config.h"
#ifdef DEBUG
#include "imu/debug.h"
#endif //DEBUG
#include <pigpiod_if2.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Macros */
#if defined (__GNUC__) || defined (__clang__)
  #define likely(x) __builtin_expect(!!x, 1)
  #define unlikely(x) __builtin_expect(!!x, 0)
#else
  #define likely(x) !!(x)
  #define unlikely(x) !!(x)
#endif

#endif //LMP_PROJECT_HARDWARE_IMU_COMMON_H_

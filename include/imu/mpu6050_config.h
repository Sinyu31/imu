#ifndef LMP_PROJECT_HARDWARE_IMU_MPU6050_CONFIG_H_
#define LMP_PROJECT_HARDWARE_IMU_MPU6050_CONFIG_H_

#include <stdint.h>

/**
 * @file  mpu6050_config.h
 * @brief register definitions, configurations, and data structs for the MPU-6050 IMU sensor
 *
*/

/* I2C address */
#define MPU6050_I2C_ADDR     0x68

/* The expected value of REGMAP_WHO_AM_I */
#define WHO_AM_I_EXPECT_0    0x68
#define WHO_AM_I_EXPECT_1    0x70
#define WHO_AM_I_EXPECT_2    0x71

/* Bus */
#define BUS_DEV_I2C_0           0
#define BUS_DEV_I2C_1           1
#define BUS_DEV_I2C_2           2

#define PWR_MGMT_WARE_UP     0x00
#define PWR_MGMT_SLEEP       0x40

/* Register map */
#define REGMAP_SMPLRATE_DIV  0x19

#define REGMAP_CONFIG        0x1A
#define REGMAP_GYRO_CONFIG   0x1B
#define REGMAP_ACCEL_CONFIG  0x1C

#define REGMAP_ACCEL_XOUT_H  0x3B
#define REGMAP_ACCEL_XOUT_L  0x3C
#define REGMAP_ACCEL_YOUT_H  0x3D
#define REGMAP_ACCEL_YOUT_L  0x3E
#define REGMAP_ACCEL_ZOUT_H  0x3F
#define REGMAP_ACCEL_ZOUT_L  0x40

#define REGMAP_TEMP_OUT_H    0x41
#define REGMAP_TEMP_OUT_L    0x42

#define REGMAP_GYRO_XOUT_H   0x43
#define REGMAP_GYRO_XOUT_L   0x44
#define REGMAP_GYRO_YOUT_H   0x45
#define REGMAP_GYRO_YOUT_L   0x46
#define REGMAP_GYRO_ZOUT_H   0x47
#define REGMAP_GYRO_ZOUT_L   0x48

#define REGMAP_PWR_MGMT_1    0x6B
#define REGMAP_PWR_MGMT_2    0x6C

#define REGMAP_WHO_AM_I      0x75

/* LSB sensitivity */
#define ACCEL_LSB_SENSITIVITY_2_G    16384.0f
#define ACCEL_LSB_SENSITIVITY_4_G    8192.0f
#define ACCEL_LSB_SENSITIVITY_8_G    4096.0f
#define ACCEL_LSB_SENSITIVITY_16_G   2048.0f

#define GYRO_LSB_SENSITIVITY_250_DPS  131.0f
#define GYRO_LSB_SENSITIVITY_500_DPS  65.5f
#define GYRO_LSB_SENSITIVITY_1000_DPS 32.8f
#define GYRO_LSB_SENSITIVITY_2000_DPS 16.4f

/* Per digit */
#define ACCEL_PER_DIGIT_2_G 0.000061f
#define ACCEL_PER_DIGIT_4_G 0.000122f
#define ACCEL_PER_DIGIT_8_G 0.000244f
#define ACCEL_PER_DIGIT_16_G 0.0004882f

#define GYRO_PER_DIGIT_250_DPS  0.007633f
#define GYRO_PER_DIGIT_500_DPS  0.015267f
#define GYRO_PER_DIGIT_1000_DPS 0.030487f
#define GYRO_PER_DIGIT_2000_DPS 0.060975f

typedef enum {
	SENS_ACCEL = REGMAP_ACCEL_XOUT_H, // accelerometor 
	SENS_GYRO  = REGMAP_GYRO_XOUT_H,  // gyroscope
} imu_sensor_data_t;

typedef enum {
    GYRO_250_DPS  = 0b00, // (+/-)  250 [deg / s]
    GYRO_500_DPS  = 0b01, // (+/-)  500 [deg / s]
    GYRO_1000_DPS = 0b10, // (+/-) 1000 [deg / s]
    GYRO_2000_DPS = 0b11  // (+/-) 2000 [deg / s]
} gyro_range_t;

typedef enum {
    ACCEL_2_G  = 0b00,    // (+/-)  2 [g]
    ACCEL_4_G  = 0b01,    // (+/-)  4 [g]
    ACCEL_8_G  = 0b10,    // (+/-)  8 [g]
    ACCEL_16_G = 0b11     // (+/-) 16 [g]
} accel_range_t;

typedef enum {
	DLPF_CFG_0  = 0x00, /* 260Hz accel / 256Hz gyro */
	DLPF_CFG_1  = 0x01, /* 184Hz accel / 188Hz gyro */
	DLPF_CFG_2  = 0x02, /*  94Hz accel /  98Hz gyro */
	DLPF_CFG_3  = 0x03, /*  44Hz accel /  42Hz gyro */
	DLPF_CFG_4  = 0x04, /*  21Hz accel /  20Hz gyro */
	DLPF_CFG_5  = 0x05, /*  10Hz accel /  10Hz gyro */
	DLPF_CFG_6  = 0x06, /*   5Hz accel /   5Hz gyro */
} dlpf_cfg_t;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} vec3i_t, AccelRaw, GyroRaw;

typedef struct {
	float x;
	float y;
	float z;
} vec3f_t, Accel, Gyro;

typedef struct {
    imu_sensor_data_t sens;
    float per_digit;
    vec3f_t vec;
} sensor_data_real_fast_t;

#endif //LMP_PROJECT_HARDWARE_IMU_MPU6050_CONFIG_H_

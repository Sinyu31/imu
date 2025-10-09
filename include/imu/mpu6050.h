#ifndef LMP_PROJECT_HARDWARE_IMU_MPU_6050_H_
#define LMP_PROJECT_HARDWARE_IMU_MPU_6050_H_

#include "imu/common.h"

/**
 * @file mpu6050.h
 * @brief Definitions and API for MPU-6050 IMU sensor using I2c and pigpio
*/

#ifdef __cplusplus 
extern "C" {
#endif //__cplusplus

/**
 * @brief Start an I2C session
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param bus I2C bus number
 * @param addr I2C address
 * @return I2C handle (>= 0) if OK, otherwise 
 *  RC_INVALID_BUS, RC_INVALID_I2C_ADDR, RC_RESOURCE_UNAVAILABLE, RC_FAIL_I2C_OPEN, RC_UNEXPECTED_ERROR  
*/
int i2c_begin_session(int pi, unsigned int bus, unsigned int addr);

/**
 * @brief End an I2C session
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @return RC_OK if OK, otherwise RC_FAIL_I2C_CLOSE
*/
int i2c_end_session(int pi, unsigned int handle);

/**
 * @brief Set sensor range
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param sens Sensor type (SENS_ACCEL or SENS_GYRO)
 * @param flag Range value: can be 0-3 or corresponding enum 
 *      - accel_range_t if SENS_ACCEL
 *      - gyro_range_t if SENS_GYRO
 * @return RC_OK if OK, otherwise RC_INVALID_ARGUMENT, RC_FAIL_SET
*/
int set_sensor_range(int pi, unsigned int handle, imu_sensor_data_t sens, uint8_t flag);

/**
 * @brief Get sensor range
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param sens Sensor type (SENS_ACCEL or SENS_GYRO)
 * @param[out] value pointer to store range (0-3), can be cast to enum:
 *      - accel_range_t if SNES_ACCEL
 *      - gyro_range_t if SENS_GYRO
 * @return RC_OK if OK, otherwise RC_FAIL_GET
*/
int get_sensor_range(int pi, unsigned int handle, imu_sensor_data_t sens, uint8_t* value);

/**
 * @brief Set Digital Low Pass Filter (DLPF) configuration
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param cfg DLPF configuration
 * @return RC_OK if OK, otherwise RC_FAIL_SET
*/
int set_dlpf_cfg(int pi, unsigned int handle, dlpf_cfg_t cfg);

/**
 * @brief Get Digital Low Pass Filter (DLPF) configuration
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param[out] value Pointer to store DLPF configuration
 * @return RC_OK if OK, otherwise RC_FAIL_GET
*/
int get_dlpf_cfg(int pi, unsigned int handle, dlpf_cfg_t* value);

/**
 * @brief Set sample rate divider
 * 
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param div Divider value
 * @return RC_OK if OK, otherwise RC_FAIL_SET
 */
int set_sample_rate(int pi, unsigned int handle, uint8_t div);

/**
 * @brief Get sample rate divider
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param[out] value Pointer to store divider value
 * @return RC_OK if OK, otherwise RC_FAIL_GET
*/
int get_sample_rate(int pi, unsigned int handle, uint8_t* value);

/**
 * @brief Read raw sensor data
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param sens Sensor type (SENS_ACCEL or SENS_GYRO)
 * @param[out] dst Pointer to vec3i_t struct to store raw data
 * @return RC_OK if OK otherwise RC_FAIL_GET
*/
int get_sensor_data_raw(int pi, unsigned int handle, imu_sensor_data_t sens, vec3i_t* dst);

/**
 * @brief Read sensor data in physical units
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param sens Sensor type (SENS_ACCEL or SENS_GYRO)
 * @param[out] dst Pointer to vec3f_t struct to store converted data
 * @return RC_OK if OK, otherwise RC_FAIL_GET
 */
int get_sensor_data_real(int pi, unsigned int handle, imu_sensor_data_t sens, vec3f_t* dst);

/**
 * @brief Fast read of sensor data in physical units
 *
 * This function does not check the current sensor range or sensitivity.
 * The user must provide the per_digit conversion factor in the struct.
 *
 * @param pi Pigpio handle (returned by pigpiod_daemon_open)
 * @param handle I2C session handle (returned by i2c_begin_session)
 * @param[out] data Pointer to sensor_data_real_fast_t struct 
 *      containing sensor type and per_digit factor
 * @return RC_OK if OK, otherwise RC_FAIL_GET
*/

int get_sensor_data_real_fast(int pi, unsigned int handle, sensor_data_real_fast_t* data);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //LMP_PROJECT_HARDWARE_IMU_MPU_6050_H_

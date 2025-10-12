#include "imu/mpu6050.h"
#include "imu/mpu6050_config.h"
#include <stdint.h>

static inline int read_register_8(int pi, unsigned int handle, unsigned int reg, uint8_t* value) {
    assert(value != NULL);
    assert((reg & ~0xFFu) == 0);
    
    int v = i2c_read_byte_data(pi, handle, reg);
    if (v < 0) return RC_FAIL_I2C_READ;
    
    *value = (uint8_t)v;
    return RC_OK;
}

static inline int write_register_8(int pi, unsigned int handle, unsigned int reg, uint8_t value) {
    assert((reg & ~0xFFu) == 0);

        if (i2c_write_byte_data(pi, handle, reg, value) != 0) return RC_FAIL_I2C_WRITE;
        return RC_OK;
}

static int read_data_n(int pi, unsigned int handle, unsigned int reg_start, uint8_t* buf, unsigned int n) {
    assert(n > 0 && n < 15);
    assert((reg_start & ~0xFFu) == 0);

    if (n == 1) { 
        uint8_t value = 0;
        if (read_register_8(pi, handle, reg_start, &value) != RC_OK) return RC_FAIL_I2C_READ;

        buf[0] = value;
        return 1;
    }
    else {
        uint8_t reg = (uint8_t)reg_start;

       if (i2c_write_device(pi, handle, (char*)&reg, 1) != 0)
           return RC_FAIL_I2C_READ;

       if (i2c_read_device(pi, handle, (char*)buf, n) != n) 
           return RC_FAIL_I2C_READ;
       
       return n;
    }
}

static  inline int set_accel_range(int pi, unsigned int handle, accel_range_t range) {
    uint8_t value = 0;
    const uint8_t MASK_KEEP = 0xE7; //0b11100111
    
    do {
        if (read_register_8(pi, handle, REGMAP_ACCEL_CONFIG, &value) != RC_OK) break;
        value &= MASK_KEEP;
        value |= ((uint8_t)range << 3);  
        if (write_register_8(pi, handle, REGMAP_ACCEL_CONFIG, value) != RC_OK) break;
        
        return RC_OK;    
    } while (0);
    return RC_FAIL_SET;
}

static inline int get_accel_range(int pi, unsigned int handle, uint8_t* value) {
    const uint8_t MASK_SEL = 0x18; //0b00011000
    uint8_t v = 0;

    if (read_register_8(pi, handle, REGMAP_ACCEL_CONFIG, &v) != RC_OK) return RC_FAIL_GET;
    v &= MASK_SEL;
    v >>= 3;
    *value = v;
    return RC_OK;
}

static inline int set_gyro_range(int pi, unsigned int handle, gyro_range_t range) {
    uint8_t value;
    const uint8_t MASK_KEEP = 0xE7; //0b11100111

    do {
        if (read_register_8(pi, handle, REGMAP_GYRO_CONFIG, &value) != RC_OK) break;
        value &= MASK_KEEP;
        value |= ((uint8_t)range << 3);
        if (write_register_8(pi, handle, REGMAP_GYRO_CONFIG, value) != RC_OK) break;

        return RC_OK;
    } while (0);
    return RC_FAIL_SET;
}

static inline int get_gyro_range(int pi, unsigned int handle, uint8_t* value) {
    const uint8_t MASK_SEL = 0x18; //0b00011000
    uint8_t v = 0;

    if (read_register_8(pi, handle, REGMAP_GYRO_CONFIG, &v) != RC_OK) return RC_FAIL_GET;
    v &= MASK_SEL;
    v >>= 3;
    *value = v;
    return RC_OK;
}

static inline float accel_lsb_sensitivity(accel_range_t range) {
    switch (range) {
        case ACCEL_2_G:  return ACCEL_LSB_SENSITIVITY_2_G;
        case ACCEL_4_G:  return ACCEL_LSB_SENSITIVITY_4_G;
        case ACCEL_8_G:  return ACCEL_LSB_SENSITIVITY_8_G;
        case ACCEL_16_G: return ACCEL_LSB_SENSITIVITY_16_G;
        default:         return NAN;
    }
}

static inline float gyro_lsb_sensitivity(gyro_range_t range) {
    switch (range) {
        case GYRO_250_DPS:  return GYRO_LSB_SENSITIVITY_250_DPS;
        case GYRO_500_DPS:  return GYRO_LSB_SENSITIVITY_500_DPS;
        case GYRO_1000_DPS: return GYRO_LSB_SENSITIVITY_1000_DPS;
        case GYRO_2000_DPS: return GYRO_LSB_SENSITIVITY_2000_DPS; 
        default:            return NAN;
    }
}

static inline float lsb_sensitivity(imu_sensor_data_t sens, uint8_t flag) {
    assert(flag <= 0b11);

    switch (sens) {
        case SENS_ACCEL: return accel_lsb_sensitivity(flag);
        case SENS_GYRO:  return gyro_lsb_sensitivity(flag); 
        default:         return NAN;
    }
}


int i2c_begin_session(int pi, unsigned int bus, unsigned int addr) {
    assert(pi >= 0);
    
    int handle = i2c_open(pi, bus, addr, 0);
    if (handle < 0) {
        switch (handle) {
            case PI_BAD_I2C_BUS:  return RC_INVALID_BUS;
            case PI_BAD_I2C_ADDR: return RC_INVALID_I2C_ADDR;
            case PI_NO_HANDLE:    return RC_RESOURCE_UNAVAILABLE;
            default:              return RC_FAIL_I2C_OPEN;
        }
    }
    do {
        uint8_t who_am_i = 0;
        if (read_register_8(pi, handle, REGMAP_WHO_AM_I, &who_am_i) != RC_OK) break;
        if (who_am_i != WHO_AM_I_EXPECT) break;
        if (write_register_8(pi, handle, REGMAP_PWR_MGMT_1, (uint8_t)PWR_MGMT_WARE_UP) != RC_OK) break;
            
        return handle;
    } while (0);
            
    return (i2c_end_session(pi, handle) == RC_OK) ? 
        RC_FAIL_I2C_OPEN : RC_UNEXPECTED_ERROR; 
}

int i2c_end_session(int pi, unsigned int handle) {
    assert(pi >= 0);

    (void)write_register_8(pi, handle, REGMAP_PWR_MGMT_1, (uint8_t)PWR_MGMT_SLEEP);

    if (unlikely(i2c_close(pi, handle) != 0)) return RC_FAIL_I2C_CLOSE;
    return RC_OK;
}

int set_sensor_range(int pi, unsigned int handle, imu_sensor_data_t sens, uint8_t flag) {
    assert(pi >= 0);
    assert(flag <= 0b11);

    switch (sens) {
        case SENS_ACCEL: return set_accel_range(pi, handle, (accel_range_t)flag);
        case SENS_GYRO:  return set_gyro_range(pi, handle, (gyro_range_t)flag);
        default:         return RC_INVALID_ARGUMENT;
    }
}
int get_sensor_range(int pi, unsigned int handle, imu_sensor_data_t sens, uint8_t* value) {
    assert(pi >= 0);
    assert(value != NULL);
    
    switch (sens) {
        case SENS_ACCEL: return get_accel_range(pi, handle, value);
        case SENS_GYRO:  return get_gyro_range(pi, handle, value);
        default:         return RC_INVALID_ARGUMENT;
    }
}

int set_dlpf_cfg(int pi, unsigned int handle, dlpf_cfg_t cfg) {
    assert(pi >= 0);

    uint8_t value;
    const uint8_t MASK_KEEP = 0xF8; //0b11111000
    
    do {
        if (read_register_8(pi, handle, REGMAP_CONFIG, &value) != RC_OK) break;
        value &= MASK_KEEP;
        value |= (uint8_t)cfg;
        if(write_register_8(pi, handle, REGMAP_CONFIG, value) != RC_OK) break;
        
        return RC_OK;
    } while (0);
    return RC_FAIL_SET;
}

int get_dlpf_cfg(int pi, unsigned int handle, dlpf_cfg_t* value) {
    assert(pi >= 0);
    assert(value != NULL);

    const uint8_t MASK_DLPF_CFG = 0x07; //0b00000111
    uint8_t v = 0;

    if (read_register_8(pi, handle, REGMAP_CONFIG, &v) != RC_OK) return RC_FAIL_GET;
    v &= MASK_DLPF_CFG;
    *value = (dlpf_cfg_t)v;
    return RC_OK;
}

int set_sample_rate(int pi, unsigned int handle, uint8_t div) {
    assert(pi >= 0);

    if (write_register_8(pi, handle, REGMAP_SMPLRATE_DIV, div) != RC_OK) return RC_FAIL_SET;
    return RC_OK;
}

int get_sample_rate(int pi, unsigned int handle, uint8_t* value) {
    assert(pi >= 0);
    assert(value != NULL);
    
    uint8_t v = 0;
    if (read_register_8(pi, handle, REGMAP_SMPLRATE_DIV, &v) != RC_OK) return RC_FAIL_GET;
    *value = v;
    return RC_OK;
}


int get_sensor_data_raw(int pi, unsigned int handle, imu_sensor_data_t sens, vec3i_t* dst) {
    assert(pi >= 0);
    assert(dst != NULL);

    uint8_t buf[6];
    unsigned int size = sizeof(buf);

    switch (sens) {
        case SENS_ACCEL:
            if (read_data_n(pi, handle, REGMAP_ACCEL_XOUT_H, buf, size) != size) return RC_FAIL_GET;
            break;
        case SENS_GYRO:
            if (read_data_n(pi, handle, REGMAP_GYRO_XOUT_H, buf, size) != size) return RC_FAIL_GET;
            break;
        default:
            return RC_INVALID_ARGUMENT;
    }

    dst->x = (int16_t)((buf[0] << 8) | buf[1]);
    dst->y = (int16_t)((buf[2] << 8) | buf[3]);
    dst->z = (int16_t)((buf[4] << 8) | buf[5]);
    return RC_OK;
}

int get_sensor_data_real(int pi, unsigned int handle, imu_sensor_data_t sens, vec3f_t* dst) {
    assert(pi >= 0);
    assert(dst != NULL);

    vec3i_t v_raw;
    uint8_t range;
    float sensitivity;
    do {
        if (get_sensor_data_raw(pi, handle, sens, &v_raw) != RC_OK) break;
        if (get_sensor_range(pi, handle, sens, &range) != RC_OK) break;
        sensitivity = lsb_sensitivity(sens, range);
        if (isnan(sensitivity)) break;

        dst->x = (float)v_raw.x / sensitivity;
        dst->y = (float)v_raw.y / sensitivity;
        dst->z = (float)v_raw.z / sensitivity;
    
        return RC_OK;
    } while (0);
    return RC_FAIL_GET;
}

int get_sensor_data_real_fast(int pi, unsigned int handle, sensor_data_real_fast_t* data) {
    assert(pi >= 0);
    assert(data != NULL);
    
    vec3i_t v_raw;
    if (get_sensor_data_raw(pi, handle, data->sens, &v_raw) != RC_OK) return RC_FAIL_GET;

    data->vec.x = (float)v_raw.x * data->per_digit;
    data->vec.y = (float)v_raw.y * data->per_digit;
    data->vec.z = (float)v_raw.z * data->per_digit;
    return RC_OK;
}

int get_accel_gyro_data_real_fast(int pi, unsigned int handle, sensor_data_real_fast_t* accel, sensor_data_real_fast_t* gyro) {
    assert(pi >= 0);
    assert(accel != NULL && gyro != NULL);

    uint8_t buf[12];
    unsigned int size = sizeof(buf);
    if (read_data_n(pi, handle, REGMAP_ACCEL_XOUT_H, buf, size) != size) return RC_FAIL_GET;
    
    vec3i_t accel_raw, gyro_raw;

    accel_raw.x = (int16_t)((buf[0] << 8) | buf[1]);
    accel_raw.y = (int16_t)((buf[2] << 8) | buf[3]);
    accel_raw.z = (int16_t)((buf[4] << 8) | buf[5]);

    gyro_raw.x = (int16_t)((buf[6] << 8)  | buf[7]);
    gyro_raw.y = (int16_t)((buf[8] << 8)  | buf[9]);
    gyro_raw.z = (int16_t)((buf[10] << 8) | buf[11]);

    accel->vec.x = (float)accel_raw.x * accel->per_digit;
    accel->vec.y = (float)accel_raw.y * accel->per_digit;
    accel->vec.z = (float)accel_raw.z * accel->per_digit;

    gyro->vec.x = (float)gyro_raw.x * gyro->per_digit;
    gyro->vec.y = (float)gyro_raw.y * gyro->per_digit;
    gyro->vec.z = (float)gyro_raw.z * gyro->per_digit;

    return RC_OK;
}

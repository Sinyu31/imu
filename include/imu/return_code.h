#ifndef LMP_PROJECT_HARDWARE_IMU_RETURN_CODE_H_
#define LMP_PROJECT_HARDWARE_IMU_RETURN_CODE_H_

#define RC_OK 0
#define RC_UNEXPECTED_ERROR      -1
#define RC_FAIL_I2C_OPEN         -2
#define RC_FAIL_I2C_CLOSE        -3
#define RC_FAIL_DAEMON_CONNECT   -4
#define RC_FAIL_I2C_READ         -5
#define RC_FAIL_I2C_WRITE        -6
#define RC_INVALID_ARGUMENT      -7
#define RC_INVALID_BUS           -8
#define RC_INVALID_I2C_ADDR      -9
#define RC_RESOURCE_UNAVAILABLE -10
#define RC_FAIL_SET             -11
#define RC_FAIL_GET             -12

#endif //LMP_PROJECT_HARDWARE_IMU_RETURN_CODE_H_

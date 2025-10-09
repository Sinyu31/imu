#include "imu/daemon.h"
#include "imu/mpu6050.h"

#include <stdio.h>
#include <unistd.h>

int main() {
    int pi = pigpiod_daemon_open(LOCALHOST, "8888");
    if (pi == RC_FAIL_DAEMON_CONNECT) {
        printf("Failed to connect pigpiod daemon \n");
        return -1;
    }

    int temp = i2c_begin_session(pi, BUS_DEV_I2C_1, MPU6050_I2C_ADDR);
    if (temp < 0) {
        printf("Failed to begin i2c session [status %d] \n", temp);
        pigpiod_daemon_close(pi);
        return -1;
    }

    unsigned int handle = (unsigned int)temp;

    if (set_sensor_range(pi, handle, SENS_ACCEL, ACCEL_8_G) != RC_OK) {
        printf("Failed to set accel range \n");
    }
    if (set_sensor_range(pi, handle, SENS_GYRO, GYRO_500_DPS) != RC_OK) {
        printf("Failed to set gyro range \n");
    }
    if (set_dlpf_cfg(pi, handle, DLPF_CFG_3) != RC_OK) {
        printf("Failed to set DLPF \n");
    }
    if (set_sample_rate(pi, handle, 4) != RC_OK) {
        printf("Failed to set sample rate \n");
    }

    for (int i = 0; i < 5; ++i) {
        Accel a;
        Gyro g;
        if (get_sensor_data_real(pi, handle, SENS_ACCEL, &a) == RC_OK) {
            printf("Accel: x=%.2f, y=%.2f, z=%.2f \n", a.x, a.y, a.z);
        }

        if (get_sensor_data_real(pi, handle, SENS_GYRO, &g) == RC_OK) {
            printf("Gyro: x=%.2f, y=%.2f, z=%.2f \n", g.x, g.y, g.z);
        }
        sleep(1);
    }    

    i2c_end_session(pi, handle);
    pigpiod_daemon_close(pi);

    return 0;
}

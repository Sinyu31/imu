#ifndef LMP_PROJECT_HARDWARE_IMU_DEBUG_H_
#define LMP_PROJECT_HARDWARE_IMU_DEBUG_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static inline void debug_log(FILE* fp, const char* fmt, ...) { 
    va_list args;
    va_start(args, fmt);
    (void)vfprintf(fp, fmt, args);
    va_end(args);
    (void)fflush(fp);
}
#endif //LMP_PROJECT_HARDWARE_IMU_DEBUG_H_

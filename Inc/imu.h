#ifndef IMU_H
#define IMU_H

#include <stdint.h>

typedef struct {
    uint32_t timestamp;

    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    float temp;
} IMU_Data;

#endif

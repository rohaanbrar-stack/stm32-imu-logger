#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <math.h>

void MPU6050_Init(void);
void MPU6050_Read(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz, int16_t *temp);
float MPU6050_GetMagnitude(int16_t ax, int16_t ay, int16_t az);

#endif

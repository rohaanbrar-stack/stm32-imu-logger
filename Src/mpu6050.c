#include "mpu6050.h"
#include "i2c.h"
#include <stdint.h>
#include <math.h>



void MPU6050_Init(void) {
	// Wake up sensor
	I2C_Start();
	I2C_WriteAddress(0x68, 0);
	I2C_WriteByte(0x6B); // Write to power management 1 register
	I2C_WriteByte(0x00); // Clear bit 6 to 0 to wake up sensor
	I2C_Stop();
}

void MPU6050_Read(int16_t *ax, int16_t *ay, int16_t *az, int16_t *temp, int16_t *gx, int16_t *gy, int16_t *gz) {
	uint8_t high, low;
	// Write to output registers
	I2C_Start();
	I2C_WriteAddress(0x68, 0);
	I2C_WriteByte(0x3B);
	I2C_Stop();

	// Read and combine bytes
	I2C_Start();
	I2C_WriteAddress(0x68, 1);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(1);
	*ax = (int16_t)((high << 8) | low);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(1);
	*ay = (int16_t)((high << 8) | low);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(1);
	*az = (int16_t)((high << 8) | low);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(1);
	*temp = (int16_t)((high << 8) | low);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(1);
	*gx = (int16_t)((high << 8) | low);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(1);
	*gy = (int16_t)((high << 8) | low);
	high = I2C_ReadByte(1);
	low = I2C_ReadByte(0);
	*gz = (int16_t)((high << 8) | low);
	I2C_Stop();
}

float MPU6050_GetMagnitude(int16_t ax, int16_t ay, int16_t az) {
	return sqrt(ax * ax + ay * ay + az * az);
}

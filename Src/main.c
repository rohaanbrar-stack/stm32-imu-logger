#include "mpu6050.h"
#include "i2c.h"
#include "ssd1306.h"
#include "spi.h"
#include "sd.h"
#include <stdint.h>
#include <stdio.h>

#define RCC_APB2ENR   (*(volatile uint32_t*)0x40021018)
#define GPIOC_CRH     (*(volatile uint32_t*)0x40011004)
#define GPIOC_ODR     (*(volatile uint32_t*)0x4001100C)

int main(void)
{
    // Enable clock for GPIOC
    RCC_APB2ENR |= (1 << 4);

    // Configure PC13 as output (push-pull, 2MHz)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);

    // Declarations
    int16_t ax, ay, az, temp, gx, gy, gz; // Raw data
    float gx_cal = 0.0; // -- Calibrated data --
    float gy_cal = 0.0; //
    float gz_cal = 0.0; // -- Calibrated data --
    float ax_g, ay_g, az_g, temp_c, gx_dps, gy_dps, gz_dps; // Unit converted data
    float ax_f = 0.0; // -- Cleaned data --
    float ay_f = 0.0; //
    float az_f = 0.0; //
    float gx_f = 0.0; //
    float gy_f = 0.0; //
    float gz_f = 0.0; // -- Cleaned data --
    char buffer[20]; // String array memory
    uint8_t data[512]; // SD data block memory
    uint8_t data_read[512]; // SD data block read memory
    uint8_t sd_init_conf; // SD card confirmation

    // Initializations
    I2C_Init();
    MPU6050_Init();
    SSD1306_Init();
    SSD1306_Clear();
    SSD1306_Refresh();
    SPI_Init();
    sd_init_conf = SD_Init();

    // Test sd card
    if(sd_init_conf == 0x01) SSD1306_DrawString("NO CARD", 84, 2);
    else {
    	for(int i = 0; i < 512; i++) data[i] = i % 256;
        SD_WriteBlock(0, data);
        SD_ReadBlock(0, data_read);
        int c = 0;
        for(int i = 0; i < 512; i++) {
            if(data_read[i] != data[i]) {
            	SSD1306_DrawString("FAIL", 102, 2);
            	break;
            }
            else c++;
        }
        if(c == 512) SSD1306_DrawString("PASS", 102, 2);
    }
    SSD1306_Refresh();

    // Gyro drift calibration
    for(int i = 0; i < 1000; i++) {
    	MPU6050_Read(&ax, &ay, &az, &temp, &gx, &gy, &gz); // Read sensor inputs from MPU6050

    	// Sum up values
    	gx_cal += gx;
    	gy_cal += gy;
    	gz_cal += gz;
    }
    // Divide values to get offset
    gx_cal /= 1000.0;
    gy_cal /= 1000.0;
    gz_cal /= 1000.0;

    // Program loop
    while(1) {
    	MPU6050_Read(&ax, &ay, &az, &temp, &gx, &gy, &gz); // Read sensor inputs from MPU6050

    	// Unit conversions and bias calibration
    	ax_g = (float)ax / 16384.0;
    	ay_g = (float)ay / 16384.0;
    	az_g = (float)az / 16384.0;
    	gx_dps = ((float)gx - gx_cal) / 131.0;
    	gy_dps = ((float)gy - gy_cal) / 131.0;
    	gz_dps = ((float)gz - gz_cal) / 131.0;
    	temp_c = (float)temp / 340.0 + 36.56;

    	// Sensor data cleaning (low-pass filter)
    	ax_f = 0.1 * ax_g + (1 - 0.1) * ax_f;
    	ay_f = 0.1 * ay_g + (1 - 0.1) * ay_f;
    	az_f = 0.1 * az_g + (1 - 0.1) * az_f;
    	gx_f = 0.1 * gx_dps + (1 - 0.1) * gx_f;
    	gy_f = 0.1 * gy_dps + (1 - 0.1) * gy_f;
    	gz_f = 0.1 * gz_dps + (1 - 0.1) * gz_f;

    	SSD1306_Clear(); // Clear SSD1306

    	// Store and set sensor values on SSD1306
    	sprintf(buffer, "AX: %.3f g", ax_f);
    	SSD1306_DrawString(buffer, 2, 2);
    	sprintf(buffer, "AY: %.3f g", ay_f);
    	SSD1306_DrawString(buffer, 2, 11);
    	sprintf(buffer, "AZ: %.3f g", az_f);
    	SSD1306_DrawString(buffer, 2, 20);
    	sprintf(buffer, "GX: %.3f deg", gx_f);
    	SSD1306_DrawString(buffer, 2, 29);
    	sprintf(buffer, "GY: %.3f deg", gy_f);
    	SSD1306_DrawString(buffer, 2, 38);
    	sprintf(buffer, "GZ: %.3f deg", gz_f);
    	SSD1306_DrawString(buffer, 2, 47);
    	sprintf(buffer, "Temp: %.3f C", temp_c);
    	SSD1306_DrawString(buffer, 2, 56);

    	SSD1306_Refresh(); // Refresh screen to display string
    }
}


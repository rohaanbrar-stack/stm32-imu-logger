#include "mpu6050.h"
#include "i2c.h"
#include "ssd1306.h"
#include "spi.h"
#include "sd.h"
#include "timer.h"
#include "clock.h"
#include "usart.h"
#include "ff.h"
#include "diskio.h"
#include "ffconf.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define RCC_APB2ENR   (*(volatile uint32_t*)0x40021018)
#define GPIOC_CRH     (*(volatile uint32_t*)0x40011004)
#define GPIOC_ODR     (*(volatile uint32_t*)0x4001100C)

DWORD get_fattime(void) {
	return 0;
}

int main(void)
{
    // Enable clock for GPIOC
    RCC_APB2ENR |= (1 << 4);

    // Configure PC13 as output (push-pull, 2MHz)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);

    clock_Init();

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
    float roll = 0.0; // Roll
    float pitch = 0.0; // Pitch
    uint8_t first_sample = 1; // Confirms first sample
    uint32_t prev_sample_time = 0; // Previous sample time for dt calc
    uint32_t last_timestamp; // Last timestamp for Hz update
    float dt; // Delta for roll/pitch update
    float hz = 0.0; // Hz refresh rate
    char buffer[100]; // String array memory
    uint8_t sample_count = 0; // Clock IRQ flag
    uint32_t sample_time; // Timestamp
    FATFS fs; // FATFS object for file mount
    FIL fp; // File pointer for FatFS file open
    UINT bw; // Number of bytes sent through FatFS write

    // Initializations
    TIM2_Init();
    SPI_Init();
    for(int i = 0; i < 100000; i++);
    SD_Init();
    f_mount(&fs, "0:", 1);
    f_open(&fp, "0:log.csv", FA_CREATE_ALWAYS | FA_WRITE);
    I2C_Init();
    USART_Init();
    MPU6050_Init();
    SSD1306_Init();
    SSD1306_Clear();
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

    last_timestamp = timestamp; // Initialize last timestamp for initial value

    // Program loop
    while(1) {
    	if(sample_flag) {
    		sample_flag = 0; // Reset TIM2 timer flag
    		sample_time = timestamp; // Timestamp counter
    		dt = (sample_time - prev_sample_time) * 0.01; // Delta calculation
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

    		// Roll and pitch calculations
    		if(!first_sample) {
    			roll += gx_f * dt;
    			roll = 0.98 * (roll) + 0.02 * (atan2(ay_f, az_f) * 180.0 / M_PI);
    			pitch += gy_f * dt;
    			pitch = 0.98 * (pitch) + 0.02 * (atan2(ax_f, az_f) * 180.0 / M_PI);
    		} else first_sample = 0; // Omit first sample (no prev sample for dt)
    		prev_sample_time = sample_time; // Set prev sample for dt

    		// Hz display
    		sample_count++;
    		if(sample_count == 100) {
    			sample_count = 0;
    			hz = 10000.0 / (timestamp - last_timestamp);
    			last_timestamp = timestamp;
    		}

    		// USART write
    		if(sample_count % 5 == 0) {
    			sprintf(buffer, "%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n", sample_time, ax_f, ay_f, az_f, gx_f, gy_f, gz_f, roll, pitch, temp_c);
    			for(int i = 0; buffer[i] != '\0'; i++) {
    				USART_WriteByte(buffer[i]);
    			}
    			f_write(&fp, buffer, strlen(buffer), &bw); // FatFS SD write
    		}



    		if(sample_count % 50 == 0) {
    			f_sync(&fp); // Flush FatFS write data
    			SSD1306_Clear(); // Clear SSD1306 every 50 clock ticks

    			// Store and set sensor values on SSD1306
    			sprintf(buffer, "Roll: %.3f deg", roll);
    			SSD1306_DrawString(buffer, 2, 2);
    			sprintf(buffer, "Pitch: %.3f deg", pitch);
    			SSD1306_DrawString(buffer, 2, 11);
    			sprintf(buffer, "Hz: %.0f Hz", hz);
    			SSD1306_DrawString(buffer, 2, 20);
    			sprintf(buffer, "Temp: %.3f C", temp_c);
    			SSD1306_DrawString(buffer, 2, 29);

    			SSD1306_Refresh(); // Refresh screen to display string
    		}
    	}
    }
}


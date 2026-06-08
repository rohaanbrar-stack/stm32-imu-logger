# STM32 Bare-Metal IMU Data Logger

A fully bare-metal embedded data acquisition system built on an STM32 microcontroller. This project implements custom low-level drivers (no HAL) for sensor acquisition, display output, and SD card data logging.

---

## Overview

This system reads motion data from an **MPU6050 IMU**, displays live values on an **SSD1306 OLED**, and logs sensor data to a **microSD card via SPI**.

It is designed as a learning and development platform for:
- bare-metal STM32 programming
- embedded driver design
- real-time sensor data acquisition
- embedded data logging systems

---

## Hardware

- STM32F103C8T6 (Blue Pill)
- MPU6050 IMU (I2C)
- SSD1306 OLED Display (I2C)
- MicroSD Card Module (SPI)

---

## Features

- Fully **bare-metal STM32 implementation (no HAL)**
- Custom **I2C driver**
- Custom **SPI driver**
- MPU6050 sensor driver (accelerometer + gyroscope)
- SSD1306 OLED driver for live telemetry display
- SD card logging (SPI-based FAT/raw implementation in progress)
- Raw sensor calibration and unit conversion

---

## Current Status

### Completed
- I2C and SPI communication drivers
- MPU6050 raw data acquisition
- Sensor calibration and unit conversion
- OLED live display output
- Basic SD card communication and logging structure

### In Progress
- SD card reliability and data integrity validation
- Fixed-rate sampling using hardware timers

### Planned
- Complementary filter for orientation estimation
- Roll and pitch calculation
- Timestamped data logging
- Noise and drift characterization
- PC-side data analysis pipeline
- Improved system architecture and documentation

---

## Architecture Goals

- No HAL dependencies (register-level control only)
- Modular driver-based structure
- Deterministic timing for sensor sampling
- Reliable embedded logging system
- Clean, portfolio-quality codebase

---

## Project Roadmap

1. Stabilize SD card logging and verify data integrity  
2. Implement timer-driven fixed-rate sampling  
3. Add orientation estimation (roll/pitch) using sensor fusion  
4. Improve data logging format (timestamps + structured output)  
5. Analyze sensor drift and noise behavior  
6. Prepare dataset export for PC visualization  
7. Finalize documentation and GitHub portfolio polish  

---

## Purpose

This project serves as a foundational platform for advanced embedded systems development and future work in distributed STM32-based systems (e.g., smart control systems, sensor networks).

---

## Author

Embedded systems learning project focused on low-level firmware development, real-time systems, and sensor integration.


# STM32 Bare-Metal IMU Data Logger

A fully bare-metal embedded data acquisition system built on an STM32 microcontroller. This project implements custom low-level drivers (no HAL) for sensor acquisition, timer-driven sampling, orientation estimation, display output, and SD card data logging.

---

## Overview

This system reads motion data from an **MPU6050 IMU** at a fixed 100Hz sample rate, computes **roll and pitch orientation** using a complementary filter, displays live values on an **SSD1306 OLED**, and logs sensor data to a **microSD card via SPI**.

It is designed as a learning and development platform for:
- bare-metal STM32 programming
- embedded driver design
- real-time, timer-driven sensor data acquisition
- sensor fusion / orientation estimation
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
- Custom **clock configuration** (HSI -> PLL -> 36MHz SYSCLK/APB1)
- MPU6050 sensor driver (accelerometer + gyroscope) with gyro bias calibration and low-pass filtering
- **Timer-driven fixed-rate sampling at 100Hz** (TIM2, register-level)
- **Roll and pitch estimation** via complementary filter (accelerometer + gyroscope fusion)
- **Live measured sample rate (Hz)** computed from timer ticks
- SSD1306 OLED driver for live telemetry display (roll, pitch, temperature, measured Hz)
- SD card driver (SDSC/SDHC init, block read/write, retry logic) — finalizing hardware integration

---

## Current Status

### Completed
- Clock configuration (36MHz SYSCLK, verified)
- I2C and SPI communication drivers
- MPU6050 raw data acquisition, gyro bias calibration, and low-pass filtering
- Timer-driven 100Hz fixed-rate sampling (TIM2, verified via LED toggle)
- Roll and pitch orientation estimation via complementary filter
- OLED live display of roll, pitch, temperature, and measured sample rate
- SD card driver logic (SDSC/SDHC fallback, CMD24 retry, extended timeouts) — logic verified correct, but...

### In Progress
- SD card hardware integration with an upgraded SPI module (Adafruit ADA254)
- UART output for live data streaming and PC-side characterization (in progress, USB-TTL adapter incoming)

### Planned
- UART-based live data streaming for sensor characterization (noise/drift analysis)
- Re-integrate SD logging once hardware reliability is confirmed with new module
- Structured timestamped log format (timestamp + 7 raw sensor values + roll + pitch)
- PC-side data export/analysis pipeline (CSV + Python plotting)
- Codebase cleanup (structs, naming conventions, error handling, documentation)
- Final GitHub portfolio polish (diagrams, photos/videos)

---

## Architecture Goals

- No HAL dependencies (register-level control only)
- Modular driver-based structure
- Deterministic, timer-driven sampling
- Sensor fusion for stable orientation estimates
- Reliable embedded logging system
- Clean, portfolio-quality codebase

---

## Project Roadmap

1. ~~Implement timer-driven fixed-rate sampling~~ — **complete**
2. ~~Add orientation estimation (roll/pitch) via complementary filter~~ — **complete**
3. UART-based live data streaming and sensor characterization (noise/drift)
4. Finalize SD logging with structured, timestamped records
5. PC-side dataset export and visualization (CSV + Python)
6. Codebase cleanup and documentation pass
7. Finalize GitHub portfolio polish

---

## Purpose

This project serves as a foundational platform for advanced embedded systems development and future work in distributed STM32-based systems — including an in-progress follow-on project: a multi-node wireless STM32 temperature control system (sensor nodes + servo-actuated vents + wireless mesh + web/app control).

---

## Author

Embedded systems learning project focused on low-level firmware development, real-time systems, sensor integration, and sensor fusion.

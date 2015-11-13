#ifndef __ADXL345__
#define __ADXL345__
#include <ESP8266WiFi.h>
#include <Wire.h>

#define ADXL345_DEVICE (0xD3)
#define ADXL345_POWER_CONTROL (0x2D)
#define ADXL345_DATA_FORMAT (0x31)
#define ADXL345_DATA_POINTER (0x32)
#define ADXL345_SCALE (4.0 / 1000)

#define HMC5883L_DEVICE (0x1e)
#define HMC5883L_DATA_POINTER (0x3)
#define HMC5883L_CONFIG_A (0x00)
#define HMC5883L_CONFIG_B (0x01)
#define HMC5883L_MODE (0x02)
#define HMC5883L_SCALE (1.0)

#define ITG3200_DEVICE (0x68)
#define ITG3200_DATA_POINTER (0x1b)
#define ITG3200_SAMPLE_RATE_DIV (0x15)
#define ITG3200_CONFIG (0x16)
#define ITG3200_GYRO_SCALE (1.0 / 14.375)
#define ITG3200_TEMP_SCALE (-35.0 / 13200)

enum BitOrder { LSB_MSB, MSB_LSB };
void decoder(byte* data, int size, double* results, BitOrder order);

#endif


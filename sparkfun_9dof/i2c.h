#ifndef __I2C__
#define __I2C__
#include <ESP8266WiFi.h>
#include <Wire.h>

void I2C_scan(Stream& stream);
void I2C_writeTo(int device, byte address, byte val);
void I2C_readFrom(int device, byte address, int number, byte* buffer);

#endif


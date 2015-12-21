#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include "i2c.h"
#include "sparkfun9dof.h"

#define ACCESS_POINT_NAME "iot-dataflow"
#define ACCESS_POINT_PASSWORD "it-at-jcu"

// Multicast declarations
const IPAddress ipMulti(239, 0, 0, 57);
const unsigned int portMulti = 8080;

String DEVICE_ID;

byte buffer[8];
double results[4];
String sensorData;
WiFiUDP udp;

void setup() {
  sensorData.reserve(500);
  setup9DOF();
  setupWIFI();
}

void loop() {
  sensorData.remove(0); // clear all text
  sensorData.concat(DEVICE_ID);
  sensorData.concat(" ");
  sensorData.concat(millis());
  sensorData.concat(" ");
  readADXL345();
  readHMC5883L();
  readITG3200();
  
  // send multicast reply
  udp.beginPacketMulticast(ipMulti, portMulti, WiFi.localIP());
  udp.println(sensorData);
  udp.endPacket();
  delay(50);
}

void setup9DOF() {
  Wire.begin(0, 2);

  // Note: all data is 16bit twos complement
  // put ADXL345 into 16g full resolution measurement mode
  I2C_writeTo(ADXL345_DEVICE, ADXL345_DATA_FORMAT, 0b00001011);
  I2C_writeTo(ADXL345_DEVICE, ADXL345_POWER_CONTROL, 0x08);

  // put HMC5883L into 15Hz updates, no averaging of samples,
  // default gain, and continuous measurement mode
  I2C_writeTo(HMC5883L_DEVICE, HMC5883L_CONFIG_A, 0b00010000);
  I2C_writeTo(HMC5883L_DEVICE, HMC5883L_CONFIG_B, 0b00100000);
  I2C_writeTo(HMC5883L_DEVICE, HMC5883L_MODE, 0b00000000);

  // put ITG3200 into normal operating mode, 100Hz (188Hz LP Filter)
  I2C_writeTo(ITG3200_DEVICE, ITG3200_CONFIG, 0b00011001);
  I2C_writeTo(ITG3200_DEVICE, ITG3200_SAMPLE_RATE_DIV, 9);
}

void setupWIFI() {
  WiFi.begin(ACCESS_POINT_NAME, ACCESS_POINT_PASSWORD);

  int attempts = 0;
  const int maxAttempts = 100;
  while (WiFi.status() != WL_CONNECTED) {
    ++attempts;

    if (attempts > maxAttempts) {
      return;
    }
    yield(); // yield to ESP8266 background functions
    delay(100);
  }

  udp.beginMulticast(WiFi.localIP(),  ipMulti, portMulti);
  DEVICE_ID = WiFi.localIP().toString();
  DEVICE_ID.replace(".", "");
}

void readADXL345() {
  I2C_readFrom(ADXL345_DEVICE, ADXL345_DATA_POINTER, 6, buffer);
  decoder(buffer, 3, results, LSB_MSB);

  sensorData.concat(results[0] * ADXL345_SCALE);
  sensorData.concat(" ");
  sensorData.concat(results[1] * ADXL345_SCALE);
  sensorData.concat(" ");
  sensorData.concat(results[2] * ADXL345_SCALE);
  sensorData.concat(" ");
}

void readHMC5883L() {
  I2C_readFrom(HMC5883L_DEVICE, HMC5883L_DATA_POINTER, 6, buffer);
  decoder(buffer, 3, results, MSB_LSB);

  // note: the order is X, Z, Y
  sensorData.concat(results[0] * HMC5883L_SCALE);
  sensorData.concat(" ");
  sensorData.concat(results[2] * HMC5883L_SCALE);
  sensorData.concat(" ");
  sensorData.concat(results[1] * HMC5883L_SCALE);
  sensorData.concat(" ");
}

void readITG3200() {
  I2C_readFrom(ITG3200_DEVICE, ITG3200_DATA_POINTER, 8, buffer);
  decoder(buffer, 4, results, MSB_LSB);

  // note: the order is TEMP, X, Y, Z

  sensorData.concat(results[1] * ITG3200_GYRO_SCALE);
  sensorData.concat(" ");
  sensorData.concat(results[2] * ITG3200_GYRO_SCALE);
  sensorData.concat(" ");
  sensorData.concat(results[3] * ITG3200_GYRO_SCALE);
//  sensorData.concat(" ");
//  sensorData.concat(results[0] * ITG3200_TEMP_SCALE);
}


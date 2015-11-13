#include "i2c.h"

void I2C_scan(Stream& stream) {
  byte error, address;
  int nDevices;

  stream.println("Scanning...");

  nDevices = 0;
  for (address = 0; address < 255; ++address) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      stream.print("I2C device found at address 0x");
      if (address < 16) {
        stream.print("0");
      }
      stream.print(address, HEX);
      stream.println(" !");

      ++nDevices;
    } else if (error == 4) {
      stream.print("Unknow error at address 0x");
      if (address < 16) {
        stream.print("0");
      }
      stream.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    stream.println("No I2C devices found\n");
  } else {
    stream.println("done\n");
  }
}

void I2C_writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); // start transmission to device
  Wire.write(address);             // send register address
  Wire.write(val);                 // send value to write
  Wire.endTransmission();         // end transmission
}

// Reads num bytes starting from address register on device in to buffer array
void I2C_readFrom(int device, byte address, int number, byte* buffer) {
  // send read address
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();

  // read data
  Wire.beginTransmission(device);
  Wire.requestFrom(device, number);
  int i = 0;
  while (Wire.available())        // device may send less than requested (abnormal)
  {
    buffer[i] = Wire.read();    // receive a byte
    ++i;
  }
  Wire.endTransmission();         // end transmission
}


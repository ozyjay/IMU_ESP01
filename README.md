# IMU_ESP01
Arduino IDE / ESP8266 (ESP-01) code for capturing data from sparkfun 9dof stick.

Setup
-----

The code runs on ESP-01 devices coded using the Arduino IDE with ESP8266 support (see https://github.com/esp8266/Arduino for details). After active the ESP-01 device connects to a WPA2 network (details of which are specified in the Arduino sketch code) the data from the sparkfun 9dof stick (https://www.sparkfun.com/products/10724) is sent via multicast UDP (details in the sketch code also).

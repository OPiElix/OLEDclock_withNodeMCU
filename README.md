# OLEDclock_withNodeMCU
I'm trying to use an OLED and a NodeMCU to make a OLED clock

**WARNING**: Only a little bit of code is mine. I've just set them together so they work. 
***I DONT DECLARE THE MAIN FUCTIONS AS MINE, SO DON'T BLAME ME*** .

## Libraries to use
**TIP:** ArduinoOTA.h is only optional for OTA update
- Wire library // `#include <Wire.h>`
- OLED lib (you could use Adafruit_SSD1136 and Adafruit_GFX) // `#include <U8g2lib.h>`
- OTA updating lib // `#include <ArduinoOTA.h>`
- RealTime from atom clock // `#include <NTPClient.h>`
- WiFi lib for Esp8266 // `#include <ESP8266WiFi.h>`
- (idk) It's needed for `ESP8266WiFi` and `NTPClient` // `#include <WiFiUdp.h>`
- Time library // `#include <TimeLib.h>`
- Time library (too) // `#include <time.h>`

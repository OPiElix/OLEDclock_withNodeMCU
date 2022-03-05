# OLEDclock_withNodeMCU
I'm trying to use an OLED and a NodeMCU to make a OLED clock

**WARNING**: Only a little bit of code is mine. I've just set them together so they work. 
***I DONT DECLARE THE MAIN FUCTIONS AS MINE, SO DON'T BLAME ME*** .

## Libraries to use
**TIP:** ArduinoOTA.h is only optional for OTA update

**TIP:** If you use SSD1306 then include `Adafruit_SSD1136.h` and `Adafruit_GFX.h`
- Wire library // `#include <Wire.h>`
- OLED lib (you could use Adafruit_SSD1136 and Adafruit_GFX) // `#include <U8g2lib.h>` // I'm using SH1106
- OTA updating lib // `#include <ArduinoOTA.h>`
- RealTime from atom clock // `#include <NTPClient.h>`
- WiFi lib for Esp8266 // `#include <ESP8266WiFi.h>`
- (idk) It's needed for `ESP8266WiFi` and `NTPClient` // `#include <WiFiUdp.h>`
- Time library // `#include <TimeLib.h>`
- Time library (too) // `#include <time.h>`
- RTC(DS3231) library // `#include "RTClib.h"` // you may include SPI too to avoid compiler error of `RTClib.h`
- Timer lib for regular RTC-adjust // `#include <TimerEvent.h>`

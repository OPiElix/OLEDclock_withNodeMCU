#include <Wire.h>
#include <U8g2lib.h>
//#include<Arduino.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <time.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"
#include <TimerEvent.h>
#define pixel u8g2_font_crox4hb_tr
#define profont u8g_font_profont22
#define vcr  u8g2_font_VCR_OSD_tr
#define twlg  u8g2_font_etl24thai_t
#define inr  u8g2_font_inr16_mn 
#define lab  u8g2_font_lastapprenticebold_tr
#define fub  u8g2_font_fub17_t_symbol
#define logis u8g2_font_logisoso16_tn
const unsigned int timerPeriod = 7200000;
const char *ssid     = "-";
const char *password = "-";
const char *ssid1     = "-";
const char *password1 = "-";
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int GTMOffset = 2;

WiFiUDP ntpUDP;
RTC_DS3231 RTC;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", GTMOffset*60*60, 60*60*1000);
// By default 'pool.ntp.org' is used with 60 seconds update interval and no offset
U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R0);
TimerEvent adjustTimer;
DateTime nowie = RTC.now();

String h = (String) nowie.hour();
String mi = (String) nowie.minute();
String s = (String) nowie.second();
String y = (String) nowie.year();
String mo = (String) nowie.month();
String d = (String) nowie.day();
String w_Weekday = (String) nowie.dayOfTheWeek();
String t_Time = h +"/"+ mi +"/"+ s; 
String d_Date = y +"/"+ mo +"/"+ d;

static tm getDateTimeByParams(long time){
    struct tm *newtime;
    const time_t tim = time;
    newtime = localtime(&tim);
    return *newtime;
}
static String getDateTimeStringByParams(tm *newtime, char* pattern = (char *)"%d/%m/%Y %H:%M:%S"){
    char buffer[30];
    strftime(buffer, 30, pattern, newtime);
    return buffer;
}
static String getDateString(long date, char* date_pattern = (char *)"%Y/%m/%d"){
//    struct tm *newtime;
    tm newdate;
    newdate = getDateTimeByParams(date);
    return getDateTimeStringByParams(&newdate, date_pattern);
}

void  RTCadjust_update(){
  RTC.adjust(DateTime(getDateString(now()).c_str(), timeClient.getFormattedTime().c_str()));
}

void setupOTA() {
  // ArduinoOTA.setPort(8266);// Port defaults to 8266
  //ArduinoOTA.setHostname("DoorLock_001");// Hostname defaults to esp8266-[ChipID]
  //ArduinoOTA.setPassword("00340435091302008961");  // No authentication by default
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3  // Password can be set with it's md5 value as well
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");

  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {


    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

} 
void setupWiFi_OTA () {
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.softAP(ssid);
  setupOTA();
  delay(500);
  int conectionAttempt1PastTime = millis();
  int conectionAttempt2PastTime;// = millis()
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ArduinoOTA.begin();

}

// You can specify the time server pool and the offse (in seconds), additionaly you can specify the update interval (in milliseconds).

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Start the I2C
  RTC.begin();
  oled.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  adjustTimer.set(timerPeriod, RTCadjust_update);
  setupWiFi_OTA();
  //int times = 0;
  while ( WiFi.status() != WL_CONNECTED){// && times < 2000000) {
    delay ( 500 );
    Serial.print ( "." );
    //times++;
  }/*
  if( WiFi.status() != WL_CONNECTED && times > 2000000){
    WiFi.disconnect();
    WiFi.begin(ssid1, password1);
    WiFi.softAP(ssid1);
  } */
 timeClient.begin();
  delay ( 1000 );
  if (timeClient.update()){
     Serial.print ( "Adjust local clock" );
     unsigned long epoch = timeClient.getEpochTime();
     // HERE I'M UPDATE LOCAL CLOCK
     setTime(epoch);
     Serial.println(d_Date + " " + t_Time + " " + w_Weekday);
  }else{
    Serial.print ( "NTP Update not WORK!!" );
    oled.firstPage();
    do{
      oled.setFont(pixel);
      oled.drawStr(43,25,"error");
    }while(oled.nextPage());
  }
  oled.clear();
}
  
void loop() {
  adjustTimer.update();
  ArduinoOTA.handle();
    oled.firstPage();
    do{
      if(t_Time == "6:30(00)"){
        oled.clearDisplay();
        oled.setFont(profont);
        oled.drawStr(10,60,"WAKE UP");
        delay(3000);
        oled.clearDisplay();
      }else if(t_Time == "21:30(00)"){
        oled.clearDisplay();
        oled.setFont(profont);
        oled.drawStr(10,60,"SLEEP");
        delay(3000);
        oled.clearDisplay();
      }else if(t_Time == "21:30(10)"){
        oled.clearDisplay();
        oled.setFont(profont);
        oled.drawStr(7,17,"it works");
        delay(3000);
        oled.clearDisplay();
      }else{
        oled.setFont(vcr);
        oled.drawStr(7,17,t_Time.c_str());
        oled.setFont(logis);
        oled.drawStr(3,40,d_Date.c_str());
        oled.setFont(profont);
        oled.drawStr(10,60,w_Weekday.c_str());
        }
    }while(oled.nextPage());
      delay(1000);
    //}
}

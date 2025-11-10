/*
* Name: övningsprojekt
* Author: Victor Huke
* Date: 2025-10-13
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Includera biblotek
#include <U8glib.h>
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>


#define LED_PIN    12
#define LED_COUNT 24


Servo myServo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
RTC_DS3231 rtc;


void setup() {
  Serial.begin(9600);
  //ring light settings
  ring.setBrightness(100);
    ring.begin();           
  ring.show();  
  //servo settings           
  myServo.attach(9);
  //oled screen settings
   u8g.setFont(u8g_font_6x10);  
  u8g.setColorIndex(1);

  // ds3231 start up
  rtc.begin();

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  // kallar på funktion som tänder ringlight
  ringlight();
  //skriver ut tid och datum på oled skärm
  oledWrite(getTime(), rtc.getTemperature());
  float temp = rtc.getTemperature(); 
  // skriver ut på seriel monitorn
  Serial.println(getTime());
  Serial.println(rtc.getTemperature());
  servoWrite(temp);
  delay(200);
}


//funktion som tar tid från ds3231
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

// funktion som skriver ut på oled skärmen
void oledWrite(String text, float temp) {
  u8g.firstPage();
  do {
    u8g.drawStr(0, 44, text.c_str());
    u8g.drawStr(0,22, String(temp).c_str());

  } while (u8g.nextPage());
}

// visar tempratur genom grader på servo
void servoWrite(float temp) {
  int servoangel = map((int)(temp * 10), 0, 500, 0, 180);
  servoangel = constrain(servoangel, 0, 180);
  myServo.write(servoangel);
}
// ringlight minut visare
void ringlight(){
  DateTime now = rtc.now();
 int numPixels = map(now.minute(),0 , 59 , 0 , 24);
  for(int i = 0; i < numPixels; i++){
    ring.setPixelColor(i, 0,200, 200, 0);
  }
  
  ring.show();
}


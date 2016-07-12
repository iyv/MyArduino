// Date and time functions using just software, based on millis() & timer

#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSBkk,k 
#endif/

int hh=0;
int mm=0;
int svet=0;
int teplo=0;
int vozduh=0;
int korm=0;

int kormI = 2;
int svetI = 3;
int teploI = 4;
int vozduhI = 5;

int MotorEn = 9;

RTC_DS1307 rtc;

void setup () {
    Serial.begin(57600);
    
    pinMode(kormI,OUTPUT);
    pinMode(svetI,OUTPUT);
    pinMode(teploI,OUTPUT);
    pinMode(vozduhI,OUTPUT);
    
    digitalWrite(kormI,HIGH);
    digitalWrite(svetI,HIGH);
    digitalWrite(teploI,HIGH);
    digitalWrite(vozduhI,HIGH);
    
    pinMode(MotorEn, INPUT);
    
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2015, 12, 26, 23, 16, 0));
    rtc.begin();
}

void loop () {

    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.println(now.day(), DEC);
    
    hh = int(now.hour());
    mm = int(now.minute());
    
    Serial.print(hh,DEC);
    Serial.print(':');
    Serial.println(mm,DEC);
       //korm
    if ((hh == 20)&(mm == 48))
      {
        digitalWrite(kormI,LOW);
        while (digitalRead(MotorEn)){}
        delay(300);
        while (!digitalRead(MotorEn)){}
        digitalWrite(kormI,HIGH);
      }    
      
    if ((hh == 20)&(mm == 55))
      {
        digitalWrite(kormI,LOW);
        while (digitalRead(MotorEn)){}
        delay(300);
        while (!digitalRead(MotorEn)){}
        digitalWrite(kormI,HIGH);
      }      
      
    delay(10000);
}

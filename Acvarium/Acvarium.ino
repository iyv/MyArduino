// Date and time functions using just software, based on millis() & timer

#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"

int hh=0;
int mm=0;

int korm=1;

int kormI = 2;
int svetI = 3;
int vozduhI = 4;
int teploI = 5;

int MotorEn = 9;

RTC_DS1307 rtc;

void KormEn(){
  digitalWrite(kormI,LOW);
  while (digitalRead(MotorEn)){}
  delay(1000);
  while (!digitalRead(MotorEn)){}
  digitalWrite(kormI,HIGH);
  korm = 0;
  Serial.println("korm off");
}

void timeKorm(){
  //Кормление
  if ((hh == 18)&(mm > 0)&(mm < 30)&(korm == 1)){
    KormEn();
    Serial.println("korm on 18.00-18.30");
  }
  
  //Обнуление факта кормления
  if ((hh >= 21)&(hh < 23)){
    korm = 1;
    Serial.println("korm reset 21-23");
  }
}

void timeSvet(){
  //освещение
  if ((hh >= 0)&(hh < 6)){
    digitalWrite(svetI,HIGH);
    Serial.println("svet off 0-6");
  }
  if ((hh == 6)&(mm > 30)){
    digitalWrite(svetI,LOW);
    Serial.println("svet on 6.30");
  }  
  if ((hh >= 7)&(hh < 21)){
    digitalWrite(svetI,LOW);
    Serial.println("svet on 7-21");
  }
  if ((hh >= 21)&(hh < 24)){
    digitalWrite(svetI,HIGH);
    Serial.println("svet off 10-15");
  }  
}

void timeVozduh(){
  //проверка времяни кормления
  if ((hh >= 0)&(hh < 11)){
    digitalWrite(vozduhI,LOW);
    Serial.println("vozduh on 0-11");
  }    
  if ((hh >= 10)&(hh < 14)){
    digitalWrite(vozduhI,HIGH);
    Serial.println("vozduh 0ff 10-14");
  }   
  if ((hh >= 14)&(hh < 17)){
    digitalWrite(vozduhI,LOW);
    Serial.println("vozduh on 14-16");
  }
  if ((hh == 17)&(mm < 50)){
    digitalWrite(vozduhI,HIGH);
    Serial.println("vozduh off 17.50");
  }  
  if ((hh == 18)&(mm > 30)){
    digitalWrite(vozduhI,LOW);
    Serial.println("vozduh on 18.30");
  }
  if ((hh >= 19)&(hh < 24)){
    digitalWrite(vozduhI,LOW);
    Serial.println("vozduh on 19-24");
  }  
}

void timeTeplo(){
  //подогрев воды
  if ((hh >= 0)&(hh < 2)){
    digitalWrite(teploI,HIGH);
    Serial.println("teplo off 0-2");
  }
  if ((hh >= 2)&(hh < 3)){
    digitalWrite(teploI,LOW);
    Serial.println("teplo on 2-3");
  }
  if ((hh >= 3)&(hh < 8)){
    digitalWrite(teploI,HIGH);
    Serial.println("teplo off 3-8");
  }
  if ((hh >= 8)&(hh < 9)){
    digitalWrite(teploI,LOW);
    Serial.println("teplo on 8-9");
  }
  if ((hh >= 9)&(hh < 14)){
    digitalWrite(teploI,HIGH);
    Serial.println("teplo off 9-14");
  }  
  if ((hh >= 14)&(hh < 15)){
    digitalWrite(teploI,LOW);
    Serial.println("teplo on 14-15");
  }
  if ((hh >= 15)&(hh < 20)){
    digitalWrite(teploI,HIGH);
    Serial.println("teplo off 15-20");
  }
  if ((hh >= 20)&(hh < 21)){
    digitalWrite(teploI,LOW);
    Serial.println("teplo on 20-21");
  }
  if ((hh >= 21)&(hh < 24)){
    digitalWrite(teploI,HIGH);
    Serial.println("teplo off 21-24");
  }
}

void setup () {
    Serial.begin(9600);
    
    pinMode(kormI,OUTPUT);
    pinMode(svetI,OUTPUT);
    pinMode(teploI,OUTPUT);
    pinMode(vozduhI,OUTPUT);
    
    digitalWrite(kormI,HIGH);
    digitalWrite(svetI,HIGH);
    digitalWrite(teploI,HIGH);
    digitalWrite(vozduhI,HIGH);
    
    pinMode(MotorEn, INPUT);
    
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
    
    timeKorm();
    timeSvet();
    timeVozduh();
    timeTeplo();
    
    delay(20000);
}

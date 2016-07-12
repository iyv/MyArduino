/**
 * An Mirf example which copies back the data it recives.
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 */
struct PultCommand{
  char LM;
  byte LZ;
  char RM;
  byte RZ;
};

struct PultCommand UpData;

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

unsigned long oldtime;
int LM1 = 2;
int LM2 = 3;
int RM1 = 4;
int RM2 = 5;

void stopCar(){
  digitalWrite(LM1, LOW);
  digitalWrite(RM1, LOW);
  analogWrite(LM2, 0);
  analogWrite(RM2, 0);
  //Serial.println("Stop car");
}

void setup(){
  Serial.begin(9600);
  Mirf.cePin = 9;
  Mirf.csnPin = 10;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");
  Mirf.payload = sizeof(UpData);
  Mirf.channel = 10;
  Mirf.config();
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  stopCar();
  Serial.println("Listening...");
 oldtime = millis(); 
}

void loop(){
  unsigned long newtime = millis();
  if ((newtime-oldtime)>3200){
    stopCar();
    oldtime = newtime;
    Serial.println("Stop car 1");
  }
  
  if(!Mirf.isSending() && Mirf.dataReady()){
    Serial.println("Got packet");
    Mirf.getData((byte *) &UpData);
    
    if (UpData.RZ > 6){
      if (UpData.LZ > 6){
        digitalWrite(LM1, HIGH);
        digitalWrite(RM1, LOW);
        analogWrite(LM2, 0);
        analogWrite(RM2, 0);
        Serial.println("Up Right");
      }
      else if(UpData. LZ < 4){
              digitalWrite(LM1, LOW);
              digitalWrite(RM1, HIGH);
              analogWrite(LM2, 0);
              analogWrite(RM2, 0);
              Serial.println("Up Left");
            }
            else {
                  digitalWrite(LM1, HIGH);
                  digitalWrite(RM1, HIGH);
                  analogWrite(LM2, 0);
                  analogWrite(RM2, 0);
                  Serial.println("Up");
                  }
    }
    else if (UpData.RZ < 4){
            if (UpData.LZ > 6){
              digitalWrite(LM1, LOW);
              digitalWrite(RM1, LOW);
              analogWrite(LM2, 255);
              analogWrite(RM2, 0);
              Serial.println("Down Rigth");
            }
            else if(UpData. LZ < 4){
                    digitalWrite(LM1, LOW);
                    digitalWrite(RM1, LOW);
                    analogWrite(LM2, 0);
                    analogWrite(RM2, 255);
                    Serial.println("Down Left");
                 }
                 else {
                    digitalWrite(LM1, LOW);
                    digitalWrite(RM1, LOW);
                    analogWrite(LM2, 255);
                    analogWrite(RM2, 255);
                    Serial.println("DOWN");
                    }
          }
          else {
                stopCar();
                Serial.println("Stop car 2");
               }
    oldtime = newtime;
  }
}

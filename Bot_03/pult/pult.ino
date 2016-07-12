 /**
 * A Mirf example to test the latency between two Ardunio.
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
 * Note: To see best case latency comment out all Serial.println
 * statements not displaying the result and load 
 * 'ping_server_interupt' on the server.
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

int UD=A0;
int RL=A1;
int x, y;
unsigned long oldtime;

void setup(){
  Serial.begin(9600);
  Mirf.cePin = 9;
  Mirf.csnPin = 10;  
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clie1");
  Mirf.payload = sizeof(UpData);
  Mirf.channel = 10;
  Mirf.config();
  UpData.LM = 'x';
  UpData.LZ = 0;
  UpData.RM = 'y';
  UpData.RZ = 0;
  pinMode(UD, INPUT);
  pinMode(RL, INPUT);
  Serial.println("Beginning ... ");
  oldtime = millis();
}

void loop(){
  unsigned long newtime = millis();
  x = map(analogRead(UD),0,1023,0,10);
  y = map(analogRead(RL),0,1023,0,10);
  if ((UpData.LZ != x)||(UpData.RZ != y)||((newtime-oldtime)>3000)){
    UpData.LZ = x;
    UpData.RZ = y;
    oldtime = newtime;
    
    Mirf.setTADDR((byte *)"serv1");
    Mirf.send((byte *)&UpData);
    while(Mirf.isSending()){
    }
    Serial.println("Send new data...");
    /*
    Serial.println(oldtime);
    Serial.println(newtime);
    Serial.println(x);
    Serial.println(y);
    */
    delay(100);
  }
} 
  
  
  

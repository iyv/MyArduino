
#include <OneWire.h>

OneWire  ds(12);  // линия 1-Wire будет на pin 12

byte i;
byte data[12];
byte type_s;
byte addr[8];
byte addr1[8]= {0x28, 0x3C, 0x5B, 0x47, 0x02, 0x00, 0x00, 0x42}; 
byte addr2[8]= {0x28, 0x75, 0x10, 0x60, 0x04, 0x00, 0x00, 0x1F};
float celsius;

void setup(void) 
{
  Serial.begin(57600);
}

void loop(void) 
{
  if (Serial.available() > 0)  
  {  
    char inByte = Serial.read();  
    switch (inByte)
    {
      case '1':
        for( i = 0; i < 8; i++) {
          addr[i] = addr1[i];
        }
        printTEMP();
     break;
      case '2':
        for( i = 0; i < 8; i++) {
          addr[i] = addr2[i];
        }
        printTEMP();
     break;
    }
  }
}  


void printTEMP()
{  
  if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n");
    return;
    }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // запускаем конвертацию
  delay(800);     // скорее всего достаточно 750ms 
  // we might do a ds.depower() here, but the reset will take care of it.
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);                   // считываем ОЗУ датчика
  for ( i = 0; i < 9; i++) {        // обрабатываем 9 байт
    data[i] = ds.read();
  }
  // высчитываем температуру :)
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  Serial.println(celsius);
}     
       
    


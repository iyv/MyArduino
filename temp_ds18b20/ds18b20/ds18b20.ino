#include <OneWire.h>

/* 
 * получаем температуру от DS18S20 
 */

OneWire  ds(12);  // линия 1-Wire будет на pin 10

void setup(void) 
{
  Serial.begin(9600);
}


void loop(void) 
{
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }
  
  Serial.print("Address=");
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print("-");
  }  
  Serial.println();

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }
  
  if ( addr[0] != 0x28) {
      Serial.println("Device is not a DS18S20 family device.\n");
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // запускаем конвертацию
  
  delay(5000);     // скорее всего достаточно 750ms 
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // считываем ОЗУ датчика

  Serial.print("Presents=");
  Serial.println(present,HEX);
  for ( i = 0; i < 9; i++) {           // обрабатываем 9 байт
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(":");
  }
  Serial.println();
  Serial.println("-------------------");
  Serial.print(" CRC=");
  Serial.println( OneWire::crc8( data, 8), HEX);
  Serial.println("-------------------");
  
  // высчитываем температуру :)
  int HighByte, LowByte, TReading, Tc_100;
  LowByte = data[0];
  Serial.print("LB= ");Serial.print(LowByte,HEX);
  HighByte = data[1];
  Serial.print(" HB= ");Serial.println(HighByte,HEX);
  TReading = (HighByte << 8) + LowByte; 
  Tc_100 = TReading/2/10;
  Serial.print(" Temperatura = ");Serial.print(Tc_100);
  Serial.println();
}

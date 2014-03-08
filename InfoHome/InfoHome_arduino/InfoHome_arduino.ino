// I assume you know how to connect the DS1302 and LCD.
// DS1302:  CE pin    -> Arduino Digital 2
//          I/O pin   -> Arduino Digital 3
//          SCLK pin  -> Arduino Digital 4

#include <OneWire.h>
#include <DS1302.h>
#include <LiquidCrystal.h>
#include <dht.h>

OneWire  ds(12);  // линия 1-Wire будет на pin 12
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 6, 7, 8, 9);
/* Set the appropriate digital I/O pin connections */
uint8_t CE_PIN   = 3;
uint8_t IO_PIN   = 4;
uint8_t SCLK_PIN = 5;

/* Create buffers */
char buf[50];
char day[10];
int i = 0;

/* Create a DS1302 object */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);
// создаём объект-сенсор
DHT sensor = DHT();

byte m;
byte data[12];
byte type_s;
byte addr[8];
byte addr1[8]= {0x28, 0x3C, 0x5B, 0x47, 0x02, 0x00, 0x00, 0x42}; 
byte addr2[8]= {0x28, 0x75, 0x10, 0x60, 0x04, 0x00, 0x00, 0x1F};
float celsius;

void setup(void) 
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  sensor.attach(A0); 
  /*
  rtc.write_protect(false);
  rtc.halt(false);
  //   Tuesday, May 19, 2009 at 21:16:37.            
  Time t(2012, 3, 4, 22, 37, 0, 2);
  // Set the time and date on the chip *
  rtc.time(t);*/
  
  Serial.begin(57600);
}

void loop(void) 
{
  lcd.clear();
  if (Serial.available() > 0)  
  {  
    char inByte = Serial.read();  
    switch (inByte)
    {
      case '1':
        for( i = 0; i < 8; i++) {
          addr[i] = addr1[i];
        }
        printDS18B20();
     break;
      case '2':
        for( i = 0; i < 8; i++) {
          addr[i] = addr2[i];
        }
        printDS18B20();
     break;
    }
  }
    printDHT11();
  delay(1000);
  i = i + 1;
  if (i == 5) {
    print_DateTime();
    } 
}  


void printDS18B20()
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

void printDHT11()
{
  // метод update заставляет сенсор выдать текущие измерения
  sensor.update();
  lcd.clear();
  switch (sensor.getLastError())
    {
    case DHT_ERROR_OK:
      char msg[128];
      // данные последнего измерения можно считать соответствующими
      // методами
      sprintf(msg, "Temp. = %dC", sensor.getTemperatureInt());
      lcd.setCursor(0, 0);              
      lcd.print(msg);              
      sprintf(msg, "Humidity = %d%%",sensor.getHumidityInt());
      lcd.setCursor(0, 1);              
      lcd.print(msg);       
      break;
    case DHT_ERROR_START_FAILED_1:
      lcd.print("Error: start failed (stage 1)");
      break;
    case DHT_ERROR_START_FAILED_2:
      lcd.print("Error: start failed (stage 2)");
      break;
    case DHT_ERROR_READ_TIMEOUT:
      lcd.print("Error: read timeout");
      break;
    case DHT_ERROR_CHECKSUM_FAILURE:
      lcd.print("Error: checksum error");
      break;
    }
  delay(5000);
  i=0;
}

void print_DateTime()
{
  // Display time centered on the upper line
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
  
  // Display abbreviated Day-of-Week in the lower left corner
  lcd.setCursor(0, 1);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  
  // Display date in the lower right corner
  lcd.setCursor(6, 1);
  lcd.print(rtc.getDateStr());

  // Wait one second before repeating :)
  //delay (1000);
}

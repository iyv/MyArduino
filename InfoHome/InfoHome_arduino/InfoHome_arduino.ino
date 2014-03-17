/* I assume you know how to connect the DS1302 and LCD.

 DS1302:  CE pin    -> Arduino Digital 11
          I/O pin   -> Arduino Digital 10
          SCLK pin  -> Arduino Digital 9

  LCD RS  pin to digital pin 6
  LCD En  pin to digital pin 8
  LCD D4  pin to digital pin 2
  LCD D5  pin to digital pin 3
  LCD D6  pin to digital pin 4
  LCD D7  pin to digital pin 5
  LCD R/W pin to digital pin 7
*/

#include <OneWire.h>
#include <DS1302.h>
#include <LiquidCrystal.h>
#include <dht.h>

OneWire  ds(12);  // линия 1-Wire будет на pin 12
LiquidCrystal lcd(6, 8, 2, 3, 4, 5); // initialize the library with the numbers of the interface pins

/* Set the appropriate digital I/O pin connections */
DS1302 rtc(11, 10, 9); // Create a DS1302 object

/* Create buffers */
//char buf[50];
//char day[10];
int i = 1;


// создаём объект-сенсор
DHT sensor = DHT();

byte NMenu = 0; //переменная для хранения пункта меню
int RW = 7;//вывод разрещения записи в дисплей
byte m = 0;//простой счетчик
byte data[12];//массив данных с датчика ds18b20
byte type_s;
byte addr[8];//массив с адресом датчика ds18b20
byte addr1[8]= {0x28, 0x3C, 0x5B, 0x47, 0x02, 0x00, 0x00, 0x42}; //первый датчик ds18b20
byte addr2[8]= {0x28, 0x75, 0x10, 0x60, 0x04, 0x00, 0x00, 0x1F}; //второй датчик ds18b20
float celsius; //переменая для перевода в градусы цельсия

//--------------------------------------------------------------
void setup(void) 
{
  // set up the LCD's number of columns and rows: 
  pinMode(RW, OUTPUT);
  digitalWrite(RW, LOW);
  lcd.begin(16, 2);
  sensor.attach(A0); 

  lcd.clear();
  for (i=0; i<16; i++) {
      lcd.setCursor(i, 0);
      lcd.print(char(255));
      lcd.setCursor(16-i, 1);
      lcd.print(char(255));
      delay(100);
    };      
  /*
  rtc.writeProtect(false);
  rtc.halt(false);
  rtc.setDOW(7);        // Set Day-of-Week to FRIDAY
  rtc.setTime(16, 8, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(3, 9, 2014);   // Set the date to August 6th, 2010
  rtc.writeProtect(true);
  rtc.halt(true);
  */
 /* 
  lcd.clear();
  for (i=1; i<256; i++) {
    lcd.setCursor(1,0);
    lcd.print (i);
    lcd.setCursor(5,1);
    lcd.print(char(i));
    delay(200);
    };
  */
  Serial.begin(57600);
}

//--------------------------------------------------------------
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
  print_DateTime();  
  delay(1000);
  m = m + 1;
  if (m == 3) {
    printDHT11();
    }
  if (m == 6) {
    printDS18B20();
    m=1;
    delay(1000);
    }  
}  

//--------------------------------------------------------------
void printDS18B20()
{  
  lcd.clear();
  lcd.setCursor(0,0);
  if ( !ds.search(addr)) {
    //lcd.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  if ( OneWire::crc8( addr, 7) != addr[7]) {
        lcd.print("CRC is not valid!\n");
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
  lcd.setCursor(0, 0);
  lcd.print("Temp= ");
  lcd.print(celsius);
  lcd.print("C");
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void print_DateTime()
{
  // Display time centered on the upper line
  //lcd.setCursor(0, 0);
  //lcd.print("/");
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
  
  // Display abbreviated Day-of-Week in the lower left corner
  lcd.setCursor(0, 1);
  lcd.print(rtc.getDOWStr(1));
  
  // Display date in the lower right corner
  lcd.setCursor(7, 1);
  lcd.print(rtc.getDateStr(FORMAT_SHORT));

  // Wait one second before repeating :)
  delay (1000);
}

void print_Menu()
{
  switch (NMenu)
  {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print ("<   Set TIME   >");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print ("     Set HH");
      lcd.setCursor(0, 1);
      lcd.print ("<");
      lcd.setCursor(6, 1);
      lcd.print ("00");
      lcd.setCursor(15, 1);
      lcd.print (">");
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print ("     Set MM");
      lcd.setCursor(0, 1);
      lcd.print ("<");
      lcd.setCursor(6, 1);
      lcd.print ("00");
      lcd.setCursor(15, 1);
      lcd.print (">");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print ("     Set SS");
      lcd.setCursor(0, 1);
      lcd.print ("<");
      lcd.setCursor(6, 1);
      lcd.print ("00");
      lcd.setCursor(15, 1);
      lcd.print (">");
      break;
  }
}

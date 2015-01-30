/* Распиновка подключения экрана и часов к arduino

 DS1302:  CE pin    -> Arduino Digital 11
          I/O pin   -> Arduino Digital 10
          SCLK pin  -> Arduino Digital 9

1602:     LCD RS  pin to digital pin 6
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

//инициализация линии OneWire
OneWire  ds(12);  // линия 1-Wire будет на pin 12

//инициализация экрана 1602
LiquidCrystal lcd(6, 8, 2, 3, 4, 5);
int RW = 7;//вывод для разрещения записи в дисплей

//инициализация модуля часов
DS1302 rtc(11, 10, 9); // Create a DS1302 object

//Для датчика температуры и влажности
DHT sensor = DHT();

//переменные для отображения данных с датчиков ds18b20
byte type_s;
byte data[12]; //массив данных с датчика ds18b20
byte addr[8];  //массив с адресом датчика ds18b20
float celsius; //переменая для перевода в градусы цельсия

//данные по первому датчику
byte DatTemp1[8]= {0x28, 0x3C, 0x5B, 0x47, 0x02, 0x00, 0x00, 0x42}; //адрес первого датчика ds18b20
char NameDatTemp1[8]=" HOME: ";//наименование первого датчика

//данные по второму датчику
byte DatTemp2[8]= {0x28, 0x75, 0x10, 0x60, 0x04, 0x00, 0x00, 0x1F}; //адрес второго датчика ds18b20
char NameDatTemp2[8]="  OUT: ";//наименование второго датчика

//массив хранения новой даты и времени
char NewDateTime[21];

//--------------------------------------------------------------
void setup(void) 
{
  pinMode(RW, OUTPUT);    //инициализируем вывод управления дисплеем
  digitalWrite(RW, LOW);  //разрешаем запись в дисплей
  
  //инициализация дисплей в 16 исмволов и 2 строки
  lcd.begin(16, 2);
  
  //датчик dth11 подключен к А0 выводу
  sensor.attach(A0);
  
  //инициализируем СОМ порт
  Serial.begin(57600);
    
  //очищаем экран
  lcd.clear();
}

//--------------------------------------------------------------
void loop(void) 
{
  if (Serial.available() > 0)  
  {  
    char inByte = Serial.read();  
    switch (inByte)
    {
      case 'C':  //подстройка часов
        Serial.print("O");//подтверждения получения команды
        for (byte i=0;i<21;i++){
          NewDateTime[i]=(byte)Serial.read();
        };
        InstallClock(NewDateTime);
      break;
      case 'T':  //получение адреса датчика и передача температуры
        Serial.print("O");//подтверждения получения команды
        for (byte i=0;i<8;i++){
          addr[i]=(byte)Serial.read();
        };
        Serial.print(ReadTempDS18B20());
      break;
      case 'S':  //Поиск датчиков
        Serial.print("O");//подтверждения получения команды
      break;
      case 'N':  //
        Serial.print("O");//подтверждения получения команды
      break;
      case 'D':  //
        Serial.print("O");//подтверждения получения команды
      break;
    };
  };
}  

//-------------------------------------------------------------
void InstallClock(char txt[21])
{
  //HH:MM:SS YYYY.mm.DD W
  //21 char
  int i=0;
  int hh;
  int mm;
  int ss;
  int YY;
  int MM;
  int DD;
  int WW;
  while (Serial.available()>0)
  {
    txt[i] = Serial.read();
    i++;
  };
  hh= (int)txt[0]*10+(int)txt[1];
  mm= (int)txt[3]*10+(int)txt[4];
  ss= (int)txt[6]*10+(int)txt[7];
  YY= (int)txt[9]*1000+(int)txt[10]*100+(int)txt[11]*10+(int)txt[12];
  MM= (int)txt[14]*10+(int)txt[15];
  DD= (int)txt[17]*10+(int)txt[18];
  WW= (int)txt[20];
  rtc.writeProtect(false);
  rtc.halt(false);
  rtc.setTime(hh, mm, ss); //установка времени (час, мин, сек)
  rtc.setDate(MM, DD, YY); //установка даты (мес, день, год)
  rtc.setDOW(WW);          //установка дня недели (1-понедельник, 7-воскресенье)
  rtc.writeProtect(true);
}

//--------------------------------------------------------------
void printLCD(char txt[], int x=0, int y=0, boolean d=1, boolean cl=1)
{
  //x  - position cursor (0(left char)-15(right char))
  //y  - position cursor (0(up line)-1(down line))
  //d  - enable delay (default - enable)
  //cl - clear display (default - enable)
  if (cl) { lcd.clear();};
  lcd.setCursor(x,y);
  lcd.print(txt);
  if (d) { delay(TimeDelay); };
}

//--------------------------------------------------------------
void ReadAddrDS18B20()
{
  if ( !ds.search(addr)) {
    Serial.println(0);
    ds.reset_search();
    delay(250);
    return;
  }
  else{
    Serial.println(addr);
  };
}

//--------------------------------------------------------------
float ReadTempDS18B20()
{  
  if ( OneWire::crc8( addr, 7) != addr[7]) {
        //lcd.print("CRC is not valid!\n");
    return(0.0);
    }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // запускаем конвертацию
  delay(800);     // скорее всего достаточно 750ms 
  // we might do a ds.depower() here, but the reset will take care of it.
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);                   // считываем ОЗУ датчика
  for ( int i = 0; i < 9; i++) {        // обрабатываем 9 байт
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
  return(celsius);
}

//--------------------------------------------------------------
void printDHT11()
{
  // метод update заставляет сенсор выдать текущие измерения
  char msg[16];
  sensor.update();
  switch (sensor.getLastError())
    {
    case DHT_ERROR_OK:
      // данные последнего измерения можно считать соответствующими
      // методами
      sprintf(msg, "Temp. = %dC", sensor.getTemperatureInt());
      printLCD(msg,6,0,0);      
      sprintf(msg, "Humidity = %d%%",sensor.getHumidityInt());      
      printLCD(msg,6,1,1,1);
      break;
    case DHT_ERROR_START_FAILED_1:
      sprintf(msg, "Error:sf(st. 1)");
      printLCD(msg,6,1);
      break;
    case DHT_ERROR_START_FAILED_2:
      sprintf(msg, "Error:sf(st. 2)");
      printLCD(msg,6,1);      
      break;
    case DHT_ERROR_READ_TIMEOUT:
      sprintf(msg, "Error:timeout");
      printLCD(msg,6,1);      
      break;
    case DHT_ERROR_CHECKSUM_FAILURE:
      sprintf(msg, "Error:check err");
      printLCD(msg,6,1); 
      break;
    }
}

//--------------------------------------------------------------
void print_DateTime()
{
  //print Time
  printLCD(rtc.getTimeStr(),4,0,0);
  // print Day-of-Week
  printLCD(rtc.getDOWStr(1),0,1,0,0);
  //print Date
  printLCD(rtc.getDateStr(FORMAT_SHORT),7,1,1,0);
}



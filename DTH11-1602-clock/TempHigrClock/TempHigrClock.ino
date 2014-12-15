/*
Ivanov Y.V. Home Control

2014-12-11 -выбор команд
*/

/*
Команды для управления:
d - запрос данных с датчика ds18b20 (d0000000000000000)
t - данные даты и времени для текущей установки(t0000000000000)
u - запрос адресов подключенных датчиков ds18b20 (опрос датчиков)
e - вывод текста на экран (нижняя строка 16 символов)
h - запрос данных по датчику DHT11 (температура, влажность)
*/

#include <stdio.h>
#include <string.h>
#include <DS1302.h>
#include <LiquidCrystal.h>
#include <dht.h>

/* Set the appropriate digital I/O pin connections */
uint8_t CE_PIN   = 3;
uint8_t IO_PIN   = 4;
uint8_t SCLK_PIN = 5;

/* Переменные буферные */
//char buf[50]; //
//char day[10]; //
int ArrayDateTime[7]={1,2,3,4,5,6,2014}; //
char temp[50];

//адреса датчиков по умолчанию
byte addr1[8]= {0x28, 0x3C, 0x5B, 0x47, 0x02, 0x00, 0x00, 0x42}; //первый датчик ds18b20
byte addr2[8]= {0x28, 0x75, 0x10, 0x60, 0x04, 0x00, 0x00, 0x1F}; //второй датчик ds18b20

//Создаем объект для экрана
LiquidCrystal lcd(12, 11, 6, 7, 8, 9);
//Создаем объект для часов DS1302
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);
//Создаём объект-сенсор DHT
DHT sensor = DHT();

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Clock + ");
  sensor.attach(A0); 
  Serial.begin(9600);
  delay(1000);
}

void install_Date (int TD[7])
{
  //по умолчанию инициализируется 
  //понедельник, 02:03:04, 05.06.2014
  rtc.writeProtect(false);
  rtc.halt(false);
  rtc.setDOW(TD[0]);
  rtc.setTime(TD[1], TD[2], TD[3]);
  rtc.setDate(TD[4], TD[5], TD[6]);
  rtc.writeProtect(true);  
}

void loop() {
  // put your main code here, to run repeatedly: 
  lcd.clear(); 
  if (Serial.available() > 0)  
  {  
    char inByte = Serial.read();  
    switch (inByte)
    {
      case 'd'://запрос данных датчика
 
        break;
      case 'u'://обновить список датчиков ds18b20
      
      break;
      case 'e'://выполнить действие
      
      break;
      case 't'://установка даты и времени
        byte i=0;
        while (Serial.available()>0)
          {
            temp[i]=Serial.read();
            i++;
          };
        ArrayDateTime[0]=conv_to_int('0',temp[0]);
        ArrayDateTime[1]=conv_to_int(temp[1],temp[2]);
        ArrayDateTime[2]=conv_to_int(temp[3],temp[4]);
        ArrayDateTime[3]=conv_to_int(temp[5],temp[6]);
        ArrayDateTime[4]=conv_to_int(temp[7],temp[8]);
        ArrayDateTime[5]=conv_to_int(temp[9],temp[10]);
        ArrayDateTime[6]=2000+conv_to_int(temp[11],temp[12]);
        install_Date(ArrayDateTime);
        lcd.setCursor(1, 0);              
        lcd.print('  Time correct  ');
      break;
    }
  }
}

int conv_to_int(char num1, char num2) {
  int result;
  result=10*(int(num1)-48)+(int(num1)-48);
  return result;
}

void print_DS1302()
{
  //lcd.clear();//Очистка экрана
  lcd.setCursor(4, 0);//Позиция курсора: 5 символ в 1 строке
  lcd.print(rtc.getTimeStr()); //Вывод времени
  lcd.setCursor(0, 1);//Позиция курсора: 1 символ во 2 строке
  lcd.print(rtc.getDOWStr(1)); //Вывод дня недели
  lcd.setCursor(7, 1); //Позиция курсора: 8 символ во 2 строке
  lcd.print(rtc.getDateStr(FORMAT_SHORT)); //Вывод даты
}

void print_DHT()
{
  // метод update заставляет сенсор выдать текущие измерения
  sensor.update();
  //lcd.clear();//Очистка экрана
  //проверка ответа датчика на запрос данных
  switch (sensor.getLastError())
    {
    case DHT_ERROR_OK: //Ответ положительный
      char msg[128];
      // данные последнего измерения можно считать соответствующими методами
      sprintf(msg, "Temp. = %dC", sensor.getTemperatureInt());
      lcd.setCursor(0, 0);              
      lcd.print(msg);              
      sprintf(msg, "Humidity = %d%%",sensor.getHumidityInt());
      lcd.setCursor(0, 1);              
      lcd.print(msg);       
      break;
      
    case DHT_ERROR_START_FAILED_1: //ошибка при чтении с датчика
      lcd.print("Error: start failed (stage 1)");
      break;
      
    case DHT_ERROR_START_FAILED_2: //ошибка при чтении с датчика
      lcd.print("Error: start failed (stage 2)");
      break;
    case DHT_ERROR_READ_TIMEOUT: //ошибка времени ожидания ответа от датчика
      lcd.print("Error: read timeout");
      break;
      
    case DHT_ERROR_CHECKSUM_FAILURE://ошибка контрольной суммы
      lcd.print("Error: checksum error");
      break;
    }
}

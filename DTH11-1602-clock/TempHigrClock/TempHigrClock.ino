#include <stdio.h>
#include <string.h>
#include <DS1302.h>
#include <LiquidCrystal.h>
#include <dht.h>

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

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  //
  sensor.attach(A0); 
  Serial.begin(9600);
/*
  rtc.write_protect(false);
  rtc.halt(false);
  //   Tuesday, May 19, 2009 at 21:16:37.            
  Time t(2012, 3, 4, 22, 37, 0, 2);
  // Set the time and date on the chip *
  rtc.time(t);*/

}

void loop() {
  // put your main code here, to run repeatedly: 
  lcd.clear();
  print_time();
  delay(1000);
  i = i + 1;
  if (i == 5) {
    printTemp();
    } 
}

void print_time()
{
  /* Get the current time and date from the chip */
  Time t = rtc.time();
  lcd.setCursor(3, 0);
  /* Format the time and date and insert into the temporary buffer */
  snprintf(buf, sizeof(buf), "%02d-%02d-%04d",
           t.date, t.mon, t.yr);
  lcd.print(buf);
  
  lcd.setCursor(4, 1);
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
           t.hr, t.min, t.sec);
  lcd.print(buf);
  
}

void printTemp()
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

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include "RF24.h"

#define TIME_STOP  "The DS1307 is stopped.  Please initialize the time and date."
#define TIME_ERROR "DS1307 read error!  Please check the circuitry."
#define STR_RAZDEL "=================="
#define VOZD_ON "Vozd ON"
#define VOZD_OFF "Vozd OFF"
#define SVET_ON "Svet ON"
#define SVET_OFF "Svet OFF"
#define TERM_ON "Term ON"
#define TERM_OFF "Term OFF"

//распределние PIN-ов Arduino
int pinGerkon = 2;    //контакт датчика положения кормушки (геркон)
int pinKorm = 3;      //контакт реле включения кормушки
int pinSvet = 4;      //контакт реле включения света 220В
int pinVozduh = 5;    //контакт реле включения фильтра
int pinTerm = 6;      //контакт реле включения подогрева воды
RF24 radio(7,8);      //контакты подключения радиомодуля (SPI)
int pinLamp = 9;      //ШИМ управление подсветкой 12В (полевой транзистор)
int pinDs18b20 = 10;  //контакт датчика температуры (DS18b20)
int pinFotores = A0;////контакт датчика внешнего освещения аквариума (фоторезистор)
int pinButtonTerm = A1;//включение/выключение подогрева воды (кнопка)
int pinButtonSvet = A2;//включение/выключение освещения в аквариуме (кнопка)

unsigned long timeUpdate = 0;//время последнего обновления данных по времени

//переменные хранения состояний
bool SvetEn = false;      //переменная хранения текущего состояния освещения
bool BlockSvet = false;   //переменная блокирования переключения света по расписанию
bool TermEn = false;      //переменная хранения текущего состояния подогрева
bool BlockTerm = false;   //переменная блокирования переключения подогрева по расписанию
bool VozduhEn = false;    //переменная хранения текущего состояния фильрования
bool BlockVozduh = false; //переменная блокирования переключения фильтрования по расписанию
bool KormEn = false;      //индикация о выполненном кормлении в периоде

tmElements_t TimeData;    //объект хранения времени

byte addresses[][6] = {"1Node","2Node"};

//****************************************************************************
//добавление 0 к числу с одним знаком
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

//****************************************************************************
//печать даты и времени в последовательный порт
void printTimeData(){
  Serial.println(STR_RAZDEL);
  print2digits(TimeData.Hour);
  Serial.write(':');
  print2digits(TimeData.Minute);
  Serial.write(':');
  print2digits(TimeData.Second);
  Serial.print(", ");
  Serial.print(TimeData.Day);
  Serial.write('.');
  Serial.print(TimeData.Month);
  Serial.write('.');
  Serial.print(tmYearToCalendar(TimeData.Year));
  Serial.println();
}

//****************************************************************************
void setup() {
  Serial.begin(9600);
  pinMode(pinSvet, OUTPUT);
  digitalWrite(pinSvet, HIGH);
  pinMode(pinKorm, OUTPUT);
  digitalWrite(pinKorm, HIGH);
  pinMode(pinTerm, OUTPUT);
  digitalWrite(pinTerm, HIGH);
  pinMode(pinVozduh, OUTPUT);
  digitalWrite(pinVozduh, HIGH);
  pinMode(pinLamp, OUTPUT);
  digitalWrite(pinLamp, LOW);
  
  pinMode(pinFotores, INPUT);
  pinMode(pinButtonTerm, INPUT);
  pinMode(pinButtonSvet, INPUT);
  pinMode(pinGerkon, INPUT);
  Serial.println("Pin init");

  timeEvent();
  
  //включение nRF24l01
  Serial.println("Radio start");
  radio.begin();
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  // Start the radio listening for data
  radio.startListening();
  Serial.println("Programm start....");
  Serial.println(STR_RAZDEL);
}

//****************************************************************************
bool diapazon(int startH, int startM, int endH, int endM){
  //проверка на вхождение в диапазон времени
  int TH = TimeData.Hour;
  int TM = TimeData.Minute;
  int timeT = TH*60+TM;
  int timeS = startH*60+startM;
  int timeE = endH*60+endM;
  //если время начала периода больше времени окончания периода, то проверяем от времени старта до 24 часов и от 0 часов до времени окончания
  if (timeS > timeE){
    if(((timeT >= timeS) && (timeT <=1440)) || ((timeT >= 0) && (timeT <= timeE))){
      return true;
    }
    else{
      return false;
    }
  }
  //иначе проверяем чтоб время входила в промежуток от времни старта до времени окончания
  else{
    if((timeT >= timeS) && (timeT <= timeE)){
      return true;
    }
    else{
      return false;
    }
  }
}

//****************************************************************************
void nRFEvent(){
  //new nRF message
}

//****************************************************************************
void rs232Event(){
  //new rs-232 message
  //format new time
  //t17:54:00-22/07/2016
  byte numSimb = 0;
  if (Serial.available() > 0){
    char Simb = Serial.read();
    numSimb++;
    int h = 0;
    int m = 0;
    int s = 0;
    int dayS = 0;
    int mesS = 0;
    int yearS = 0;
    if (Simb == 't'){
      while (numSimb < 20){
        while(Serial.available() == 0){};
        int Simb = Serial.read()-48;
        switch (numSimb){
          case 1:
            h += Simb *10;
          break;
          case 2:
            h += Simb;
          break;
          case 4:
            m += Simb*10;
          break;
          case 5:
            m += Simb;
          break;
          case 7:
            s += Simb*10;
          break;
          case 8:
            s += Simb;
          break;
          case 10:
            dayS += Simb*10;
          break;
          case 11:
            dayS += Simb;
          break;
          case 13:
            mesS += Simb*10;
          break;
          case 14:
            mesS += Simb;
          break;
          case 16:
            yearS += Simb*1000;
          break;
          case 17:
            yearS += Simb*100;
          break;
          case 18:
            yearS += Simb*10;
          break;
          case 19:
            yearS += Simb;
          break;
        }
        //Serial.print(numSimb);
        //Serial.print(" - ");
        //Serial.println(Simb);
        numSimb++;
      }
      TimeData.Hour = h;
      TimeData.Minute = m;
      TimeData.Second = s;
      TimeData.Day = dayS;
      TimeData.Month = mesS;
      TimeData.Year = yearS-1970;
      RTC.write(TimeData);
      Serial.println("Install date/time");
      Serial.println(STR_RAZDEL);
    }
  }
}


//****************************************************************************
void ControlTerm(bool SwTerm, bool ButtonS = false){
  //управление подогревом
  //Serial.println("Term control");
  if (TermEn == SwTerm){
    BlockTerm = false;
  }
  if (BlockTerm){   //если переключение света заблокировано
    if (ButtonS){   //если режим переключения отправлен кнопкой
      if (SwTerm){
        //включение освещения
        digitalWrite(pinTerm,LOW);
      }
      else{
        //выключение освещения
        digitalWrite(pinTerm,HIGH);
      }
      //присваиваем текущему уровню освещения требуемое
      TermEn = SwTerm;
    }
  }
  else{
    //переключение света по программе
    //если текущеее освещение не равно требуемому
    if(SwTerm != TermEn){
      if (SwTerm){
        //включение освещения
        digitalWrite(pinTerm,LOW);
      }
      else{
        //выключение освещения
        digitalWrite(pinTerm,HIGH);
      }
      //присваиваем текущему уровню освещения требуемое
      TermEn = SwTerm;
    }
  }
}

//****************************************************************************
void ControlVozduh(bool SwVozduh, bool ButtonS = false){
  //управление воздухом
  //Serial.println("Vozduh control");
  if (VozduhEn == SwVozduh){
    BlockVozduh = false;
  }
  if (BlockVozduh){   //если переключение света заблокировано
    if (ButtonS){   //если режим переключения отправлен кнопкой
      if (SwVozduh){
        //включение освещения
        digitalWrite(pinVozduh,LOW);
      }
      else{
        //выключение освещения
        digitalWrite(pinVozduh,HIGH);
      }
      //присваиваем текущему уровню освещения требуемое
      VozduhEn = SwVozduh;
    }
  }
  else{
    //переключение света по программе
    //если текущеее освещение не равно требуемому
    if(SwVozduh != VozduhEn){
      if (SwVozduh){
        //включение освещения
        digitalWrite(pinVozduh,LOW);
      }
      else{
        //выключение освещения
        digitalWrite(pinVozduh,HIGH);
      }
      //присваиваем текущему уровню освещения требуемое
      VozduhEn = SwVozduh;
    }
  }
}

//****************************************************************************
void ControlSvet(bool SwSvet, bool ButtonS = false){
  //управление светом
  //Serial.println("Svet control");
  if (SvetEn == SwSvet){
    BlockSvet = false;
  }
  if (BlockSvet){   //если переключение света заблокировано
    if (ButtonS){   //если режим переключения отправлен кнопкой
      if (SwSvet){
        //включение освещения
        digitalWrite(pinSvet,LOW);
      }
      else{
        //выключение освещения
        digitalWrite(pinSvet,HIGH);
      }
      //присваиваем текущему уровню освещения требуемое
      SvetEn = SwSvet;
    }
  }
  else{
    //переключение света по программе
    //если текущеее освещение не равно требуемому
    if(SwSvet != SvetEn){
      if (SwSvet){
        //включение освещения
        digitalWrite(pinSvet,LOW);
      }
      else{
        //выключение освещения
        digitalWrite(pinSvet,HIGH);
      }
      //присваиваем текущему уровню освещения требуемое
      SvetEn = SwSvet;
    }
  }
}

//****************************************************************************
void ControlKorm(){
  //кормление
  //Serial.println("Korm control");
  digitalWrite(pinKorm,LOW);
  delay(200);
  while (!digitalRead(pinGerkon)){}
  digitalWrite(pinKorm,HIGH);
  KormEn = true;
}

//****************************************************************************
void buttonEvent(){
  //нажата кнопка подогрева
  //Serial.println("Button event");
  if (digitalRead(pinButtonTerm)){
    BlockTerm = true;
    //если подогрев включен, то выключаем его
    if (TermEn){
      ControlTerm(false, true);
    }
    // иначе включаем
    else{
      ControlTerm(true, true);
    }
  }
  //нажата кнопка освещения
  if(digitalRead(pinButtonSvet)){
    BlockSvet = true;
    //если освещение включено, то выключаем его
    if (SvetEn){
      ControlSvet(false, true);
    }
    // иначе включаем
    else{
      ControlSvet(true, true);
    }
  }
}

//****************************************************************************
void timeEvent(){
  //time event
  //Serial.print("Time event- ");
  //Serial.println(millis());
  if ((timeUpdate + 10000) < millis()){
    //получение текущего времени
    if (RTC.read(TimeData)) {
      //вывод даты и времени в rs-232
      printTimeData();
      timeUpdate = millis();
      
      //Serial.println("Time read");
      //diapazon(int startH, int startM, int endH, int endM)
      //СВЕТ
      if (diapazon(6, 30, 9, 59)){
        //ledOn
        ControlSvet(true);
        Serial.println(SVET_ON);
      }
      if (diapazon(10, 00, 13, 59)){
        //ledOff
        ControlSvet(false);
        Serial.println(SVET_OFF);
      }
      if (diapazon(14, 00, 21, 30)){
        //ledOn
        ControlSvet(true);
        Serial.println(SVET_ON);
      }
      if (diapazon(21, 31, 6, 29)){
        //ledOff
        ControlSvet(false);
        Serial.println(SVET_OFF);
      }

      //ВОЗДУХ
      if (diapazon(6, 30, 17, 55)){
        //filterOn
        ControlVozduh(true);
        Serial.println(VOZD_ON);
      }
      if (diapazon(17, 56, 18, 20)){
        //filterOff
        ControlVozduh(false);
        Serial.println(VOZD_OFF);
      }
      if (diapazon(18, 21, 3, 30)){
        //filterOn
        ControlVozduh(true);
        Serial.println(VOZD_ON);
      }
      if (diapazon(3, 31, 6, 29)){
        //filterOff
        ControlVozduh(false);
        Serial.println(VOZD_OFF);
      }

      //ПОДОГРЕВ
      if (diapazon(3, 00, 8, 59)){
        //termOn
        ControlTerm(true);
        Serial.println(TERM_ON);
      }
      if (diapazon(9, 00, 14, 59)){
        //termOff
        ControlTerm(false);
        Serial.println(TERM_OFF);
      }
      if (diapazon(15, 00, 20, 59)){
        //termOn
        ControlTerm(true);
        Serial.println(TERM_ON);
      }
      if (diapazon(21, 00, 2, 59)){
        //termOff
        ControlTerm(false);
        Serial.println(TERM_OFF);
      }

      //КОРМЛЕНИЕ
      if (diapazon(18, 0, 19, 59)){
        //kormOn
        if (KormEn == false){
          Serial.println("Korm ON");
          ControlKorm();
        }
      }
      if (diapazon(20, 00, 17, 59)){
        //kormOff
        if (KormEn){
          KormEn = false;
        }
      }
      
    } 
    else {
      if (RTC.chipPresent()) {
        //чип времени не инициализирован
        Serial.println(TIME_STOP);
      }
      else {
        //ошибка чтения времени
        Serial.println(TIME_ERROR);
      }
    }
  }
}

//****************************************************************************
void loop() {
  // put your main code here, to run repeatedly:
  nRFEvent();
  rs232Event();
  buttonEvent();
  timeEvent();
}

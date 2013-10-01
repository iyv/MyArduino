#include "Ultrasonic.h"

int M1 =8; // Правый мотор (направление)
int M2 =9; // Левый мотор (направление)

int E1 =2; // Правый мотор (вкл/выкл)
int E2 =3; // Левый мотор (вкл/выкл)

int led =13; //Вывод светодиода

int stepSpeed=127; //
int EnMotor=1;
int maxSpeed=50;

// Trig - 22, Echo - 23
Ultrasonic ultrasonic(22,23);

void setup()
{
 pinMode(M1, OUTPUT);
 pinMode(M2, OUTPUT);
 pinMode(E1, OUTPUT);
 pinMode(E1, OUTPUT);
 Serial1.begin(9600);
 Serial.begin(9600);
}

void loop() {
  if (Serial1.available()) {
    char inChar =Serial1.read();
    
    Serial.println(inChar);
    if (inChar=='h'){
      Serial1.println("Добро пожаловать в программу управления Bot 1.0");
      Serial1.println("       (C) IYV 2013 (01.10.2013г.)");  
      Serial1.println("-------------------------------------------------");
      Serial1.println("a - движение вперед        s-движение назад ");
      Serial1.println("z - поворот налево         x-поворот направо ");
      Serial1.println("k - повышение скорости     m-понижение скорости ");
      Serial1.println("y - автопилот              e-выход с автопилота ");
      Serial1.println("                 h - информация");
    };
    if (inChar=='a'){
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      digitalWrite(led, HIGH);
      stepSpeed=127;
      EnMotor=2;
      Serial1.println("Движение вперед");
    };
    if (inChar=='s'){
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      digitalWrite(led, HIGH);
      stepSpeed=127;
      EnMotor=1;
      Serial1.println("Движение назад");
    };
    if (inChar=='q'){
      digitalWrite(led, LOW);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      analogWrite(E1,127);
      analogWrite(E2,127);
      EnMotor=0;
      Serial1.println("Полная остановка...");
    };
    if (inChar=='x'){
      digitalWrite(led, HIGH);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      analogWrite(E1,127-maxSpeed);
      analogWrite(E2,127+maxSpeed);
      EnMotor=0;
      Serial1.println("Поворот налево");
    };
    
    if (inChar=='z'){
      digitalWrite(led, HIGH);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      analogWrite(E1,127+maxSpeed);
      analogWrite(E2,127-maxSpeed);
      EnMotor=0;
      Serial1.println("Поворот направо");
    };
   if (inChar=='k'){
      if (maxSpeed < 126) {
        maxSpeed++;
        Serial1.print("Увеличение скорости до- ");
        Serial1.println(maxSpeed);}
        else {Serial1.println("Скорость максимальна");};
    };
   if (inChar=='m'){
      if (maxSpeed > 5) {
        maxSpeed--;
        Serial1.print("Уменьшение скорости до- ");
        Serial1.println(maxSpeed);}
        else {Serial1.println("Скорость минимальная");};
    };
   if (inChar=='y'){
     Serial1.println("Режим автопилота запущен!"); 
     AutoPilot();
     Serial1.println("Перевод в ручной режим управления"); 
   };    
  };

if (EnMotor==2){
  if (stepSpeed > (127-maxSpeed)) { stepSpeed--;}
    else {stepSpeed = (127-maxSpeed);}; 
};

if (EnMotor==1){
  if (stepSpeed < (127+maxSpeed)) { stepSpeed++;}
    else {stepSpeed = (127+maxSpeed);}; 
};

//Serial.println(stepSpeed);
if (EnMotor != 0) {
  analogWrite(E1,stepSpeed);
  analogWrite(E2,stepSpeed);
  };
}

void AutoPilot(){
  int SpeedAP=50;  
  while(1){
    if (Serial1.available()) {
      char inChar =Serial1.read();
      if (inChar == 'e') { return;
    }}
  float dist = ultrasonic.Ranging(CM);
  Serial1.print("Дальность- ");
  Serial1.println(dist);
  if (dist > 15.0){
    digitalWrite(M1, HIGH);
    digitalWrite(M2, HIGH);
    digitalWrite(led, HIGH);
    analogWrite(E1,127+SpeedAP);
    analogWrite(E2,127+SpeedAP);
    }
  else{
    Serial1.println("Найдена препятствие");
    while (dist < 40.0){
      if (Serial1.available()) {
        char inChar =Serial1.read();
        if (inChar == 'e') { return ;
        }
      }
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      analogWrite(E1,127+SpeedAP);
      analogWrite(E2,127-SpeedAP);      
      dist = ultrasonic.Ranging(CM);  
    }
    }
}
}
    

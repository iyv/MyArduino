/*
Бот
2-я версия. Подключен мотор-шилд.
Управление через RS-232(TTL). Автопилот отключен.
Последнее редактирование:2014
Автор: IYV
*/

//#include "Ultrasonic.h"

int M1 =3; // Правый мотор (направление)
int M2 =6; // Левый мотор (направление)

int E1 =2; // Правый мотор (вкл/выкл)
int E2 =5; // Левый мотор (вкл/выкл)

int led =13; //Вывод светодиода

int stepSpeed=127; //
int EnMotor=1;
int maxSpeed=120;

// Trig - 22, Echo - 23
//Ultrasonic ultrasonic(22,23);

void setup()
{
 pinMode(M1, OUTPUT);
 pinMode(M2, OUTPUT);
 pinMode(E1, OUTPUT);
 pinMode(E1, OUTPUT);
 pinMode(led, OUTPUT);
 Serial1.begin(9600);
 Serial.begin(9600);
}

void loop() {
  if (Serial1.available()) {
    char inChar =Serial1.read();
    
    Serial.println(inChar);
    //menu
    if (inChar=='u'){
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      digitalWrite(led, HIGH);
      digitalWrite(E1, HIGH);
      digitalWrite(E2, HIGH);
      //analogWrite(E1,127+maxSpeed);
      //analogWrite(E2,127+maxSpeed);
      //stepSpeed=127;
      //EnMotor=2;
      Serial1.println("Up");
    };
    if (inChar=='d'){
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      digitalWrite(led, HIGH);
      digitalWrite(E1, HIGH);
      digitalWrite(E2, HIGH);
      //analogWrite(E1,127-maxSpeed);
      //analogWrite(E2,127-maxSpeed);
      //stepSpeed=127;
      //EnMotor=1;
      Serial1.println("Down");
    };
    if (inChar=='q'){
      digitalWrite(led, LOW);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      digitalWrite(E1, LOW);
      digitalWrite(E2, LOW);
      //analogWrite(E1,127);
      //analogWrite(E2,127);
      //EnMotor=0;
      Serial1.println("Stop...");
    };
    if (inChar=='l'){
      digitalWrite(led, HIGH);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      digitalWrite(E1, HIGH);
      digitalWrite(E2, HIGH);
      //analogWrite(E1,127-maxSpeed);
      //analogWrite(E2,127+maxSpeed);
      //EnMotor=0;
      Serial1.println("Left");
    };
    
    if (inChar=='r'){
      digitalWrite(led, HIGH);
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      digitalWrite(E1, HIGH);
      digitalWrite(E2, HIGH);
      //analogWrite(E1,127+maxSpeed);
      //analogWrite(E2,127-maxSpeed);
      //EnMotor=0;
      Serial1.println("Right");
    };
/*   if (inChar=='k'){
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
   };  */  
  };

/*if (EnMotor==2){
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
  };*/
}

/*void AutoPilot(){
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
    Serial1.println("Найдено препятствие");
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
}*/
    

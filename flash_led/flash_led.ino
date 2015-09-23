/*
Новогодняя гирлянда 4-х цветная
Последняя редакция: 2015-01-05
Автор: IYV

LedPin - выходы к которым подключены транзисторные ключи
buttonPin - вывод к которому подключена кнопка для переключения эффектов
ArLedPin - вывод светодиода(встроенного) индикатора выбранного режима 
           (мигает по номеру выбранного режима)
*/
int LedPin[] = {3,5,6,9};
int pause1 = 2;
int pause2 = 4;
int pause3 = 100;
int NumEffect=0;
const int buttonPin = 2;
const int ArLedPin = 13;

void setup() {
  pinMode(LedPin[0],OUTPUT);
  pinMode(LedPin[1],OUTPUT);
  pinMode(LedPin[2],OUTPUT);
  pinMode(LedPin[3],OUTPUT);
  pinMode(ArLedPin,OUTPUT);
  pinMode(buttonPin,INPUT);
}

void buttonRead(){
  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {   
    if (NumEffect>5) {
      NumEffect=0;
      digitalWrite(ArLedPin,HIGH);
      delay(1000);
      digitalWrite(ArLedPin,LOW);
    }
    else {
      NumEffect++;
      for (int i=1;i<=NumEffect;i++){
        digitalWrite(ArLedPin,HIGH);
        delay(pause3);
        digitalWrite(ArLedPin,LOW);
        delay(pause3);
      }
    }
  }
}

void effect1() {
  for (int i = 0; i < 4; i++) {
    for (int level=0;level<=255;level++) {
      analogWrite(LedPin[i],level);
      buttonRead();
      delay(pause1);
    }
    for (int level=255;level>=0;level--) {
      analogWrite(LedPin[i],level);
      buttonRead();
      delay(pause1);
    }
  }
}

void effect2(){
  for (int i = 0; i < 2; i++) {
    for (int level=0;level<=255;level++) {
      analogWrite(LedPin[i],level);
      analogWrite(LedPin[i+2],level);
      buttonRead();
      delay(pause2);
    }
    for (int level=255;level>=0;level--) {
      analogWrite(LedPin[i],level);
      analogWrite(LedPin[i+2],level);
      buttonRead();
      delay(pause2);
    }
  }
}

void effect3(){
  digitalWrite(LedPin[1],HIGH);
  digitalWrite(LedPin[2],HIGH);
  digitalWrite(LedPin[3],HIGH);
  digitalWrite(LedPin[0],HIGH);
  for (int i = 0; i < 4; i++) {
    digitalWrite(LedPin[i],LOW);
    buttonRead();
    delay(pause3);
    digitalWrite(LedPin[i],HIGH);
  }
}

void effect4(){
    for (int level=0;level<=255;level++) {
      analogWrite(LedPin[0],level);
      analogWrite(LedPin[1],level);
      analogWrite(LedPin[2],level);
      analogWrite(LedPin[3],level);
      buttonRead();
      delay(pause1);
    }
    for (int level=255;level>=0;level--) {
      analogWrite(LedPin[0],level);
      analogWrite(LedPin[1],level);
      analogWrite(LedPin[2],level);
      analogWrite(LedPin[3],level);
      buttonRead();
      delay(pause1);
    }
}

void loop() {
  buttonRead();
  switch (NumEffect) {
    case 0:
      digitalWrite(LedPin[1],LOW);
      digitalWrite(LedPin[2],LOW);
      digitalWrite(LedPin[3],LOW);
      digitalWrite(LedPin[0],LOW);
      break;
    case 1:
      effect1();
      break;
    case 2:
      effect2();
      break;
    case 3:
      effect3();
      break;
    case 4:
      effect4();
      break;      
    default:
      effect1();
      effect2();
      effect3();
      effect4();
  }
}

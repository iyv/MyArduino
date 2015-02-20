/*
  Dimmer
 */
<<<<<<< HEAD

const int ledPin = 3;      // the pin that the LED is attached to
=======
#include <IRremote.h>

int RECV_PIN = 11;//ir datchik
IRrecv irrecv(RECV_PIN);
decode_results results;
const int ledPin = 6;      // the pin that the LED is attached to
>>>>>>> InfoHome
int level=0;
int result=0;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
<<<<<<< HEAD
=======
  irrecv.enableIRIn(); // Start the receiver
>>>>>>> InfoHome
  Serial.write("Start programm\n\r");
};

void LedPower(){
  while(level!=result){
    if (result<level){
      level--;}
    else{
      level++;
    };
    analogWrite(ledPin,level);
    ReadCommand();
<<<<<<< HEAD
=======
    IrRead();
>>>>>>> InfoHome
    delay(200);
  };
  Serial.println("Led Ok");
};


void ReadCommand(){
  int i=0;
  char buf[3];
  if (Serial.available()){
    delay(500);
    while (Serial.available() && (i<4)){
      buf[i]=char(Serial.read());
      i=i+1;
    };
    switch(i){
      case 3:
        if (buf[0]<'0' || buf[0]>'9'){break;};
        if (buf[1]<'0' || buf[1]>'9'){break;};
        if (buf[2]<'0' || buf[2]>'9'){break;};
        result=(int(buf[0])-48)*100+(int(buf[1])-48)*10+(int(buf[2])-48);
        if (result>255){result=255;};
        break;
      case 2:
        if (buf[0]<'0' || buf[0]>'9'){break;};
        if (buf[1]<'0' || buf[1]>'9'){break;};
        result=(int(buf[0])-48)*10+(int(buf[1])-48);
        break;
      case 1:
        if (buf[0]<'0' || buf[0]>'9'){break;};
        result=(int(buf[0])-48);
    };
  };
}

<<<<<<< HEAD
void loop() {
  ReadCommand();
  if (result!=level){
    Serial.write("Led start\n\r");
    LedPower();
  };
=======
void IrRead(){
  if (irrecv.decode(&results)) {
    if (results.value==16378389){
      Serial.write("IR Led On\n\r");
      result=200;
    }
    if (results.value==16337589){
      Serial.write("IR Led off\n\r");
      result=0;
    }
    if (results.value==16368189){
      if (result<250){
        Serial.write("Led up\n\r");
        result=result+5;
      }
    }
    if (results.value==16339119){
      if (result>0){
        Serial.write("Led down\n\r");
        result=result-5;
      }
      else {result=0;}
    }
    irrecv.resume(); // Receive the next value
  }
}

void loop() {
  ReadCommand();
  IrRead();
  if (result!=level){
    Serial.write("Led start\n\r");
    LedPower();
  }
>>>>>>> InfoHome
}

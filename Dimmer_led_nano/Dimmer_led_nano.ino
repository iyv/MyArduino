/*
  Dimmer
 */

const int ledPin = 3;      // the pin that the LED is attached to
int level=0;
int result=0;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
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

void loop() {
  ReadCommand();
  if (result!=level){
    Serial.write("Led start\n\r");
    LedPower();
  };
}

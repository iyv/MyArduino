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

/*
void LedOn(){
  while(level<255){
    level++;
    analogWrite(ledPin,level);
    if (Serial.available()){
      ReadSerial();
      break;
    };
    delay(100);
  };
  digitalWrite(13,1);
};

void LedOff(){
  while(level>0){
    level--;
    analogWrite(ledPin,level);
    if (Serial.available()){
      ReadSerial();
      break;
    }
    delay(100);
  };
  digitalWrite(13,0);
};

void ReadSerial(){
  char brightness;
  // check if data has been sent from the computer:
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 255):
    brightness = char(Serial.read());
    // set the brightness of the LED:
    if (brightness =='0'){
      Serial.write("LedOff start...\n\r");
      LedOff();
      Serial.write("LedOff Ok!\n\r");
    };
    if (brightness =='1'){
      Serial.write("LedOn start...\n\r");
      LedOn();
      Serial.write("LedOn Ok!\n\r");
    };
    if (brightness =='2'){
      Serial.write("LedOn stop...\n\r");
    };
  };
}
*/

void LedPower(){
  int inc;
  if (result<level){
    inc=-1;}
  else{
    inc=1;};
  while(level!=result){
    level+=inc;
    analogWrite(ledPin,level);
    if (Serial.available()){
      ReadCommand();
      break;
    }
    delay(100);
  };
  Serial.write("Led Ok");
};


void ReadCommand(){
  int i=0;
  char buf[3];
  if (Serial.available() && (i<3)){
    buf[i++]=Serial.read();
  };
  switch(i){
    case 3:
      if (buf[0]<'0' || buf[0]>'9'){break;};
      if (buf[1]<'0' || buf[1]>'9'){break;};
      if (buf[2]<'0' || buf[2]>'9'){break;};
      result=int(buf[0])*100+int(buf[1])*10+int(buf[2]);
      if (result>255){result=255;};
      break;
    case 2:
      if (buf[0]<'0' || buf[0]>'9'){break;};
      if (buf[1]<'0' || buf[1]>'9'){break;};
      result=int(buf[0])*10+int(buf[1]);
      break;
    case 1:
      if (buf[0]<'0' || buf[0]>'9'){break;};
      result=int(buf[0]);
  };
  if (result!=level){LedPower();};
}

void loop() {
  ReadCommand();
}

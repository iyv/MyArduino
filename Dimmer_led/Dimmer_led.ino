/*
  Dimmer
 
 Demonstrates the sending data from the computer to the Arduino board,
 in this case to control the brightness of an LED.  The data is sent
 in individual bytes, each of which ranges from 0 to 255.  Arduino
 reads these bytes and uses them to set the brightness of the LED.
 
 The circuit:
 LED attached from digital pin 9 to ground.
 Serial connection to Processing, Max/MSP, or another serial application
 
 created 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Tom Igoe and Scott Fitzgerald
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Dimmer
 
 */

const int ledPin = 3;      // the pin that the LED is attached to
int level=0;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  Serial1.begin(9600);
  // initialize the ledPin as an output:
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
  Serial.write("Start programm\n");
  Serial1.write("Start programm\n");
};

void LedOn(){
  while(level<255){
    level++;
    analogWrite(ledPin,level);
    delay(100);
  };
  digitalWrite(13,1);
};

void LedOff(){
  while(level>0){
    level--;
    analogWrite(ledPin,level);
    delay(100);
  };
  digitalWrite(13,0);
};

void loop() {
  char brightness;

  // check if data has been sent from the computer:
  if (Serial1.available()) {
    // read the most recent byte (which will be from 0 to 255):
    brightness = char(Serial1.read());
    // set the brightness of the LED:
    if (brightness =='0'){
      Serial.write("LedOff start...\n");
      Serial1.write("LedOff start...\n");
      LedOff();
      Serial.write("LedOff Ok!\n");
      Serial1.write("LedOff Ok!\n");
    };
    if (brightness =='1'){
      Serial.write("LedOn start...\n");
      Serial1.write("LedOn start...\n");
      LedOn();
      Serial.write("LedOn Ok!\n");
      Serial1.write("LedOn Ok!\n");
    };
  };
}

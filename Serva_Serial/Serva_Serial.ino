// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 

int pos =0; 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

 
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
 myservo.write(0); 
} 
 
 
void loop() 
{ 
  if (Serial.available()>0)
  {
    int key = Serial.read();
    if ((key == 49) and (pos < 170)) 
    {
      pos+=1;
      myservo.write(pos);
      
    }
    if ((key == 48) and (pos > 1)) 
    {
      pos-=1;
      myservo.write(pos);
    }
    Serial.println(pos);
  }
} 

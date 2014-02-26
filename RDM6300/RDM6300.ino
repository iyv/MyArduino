int led = 13;
byte buffer[14];

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  if (Serial1.available()){   //ждем пока появится символ для приема
    for (int i=0;i<14;i++) {
      buffer[i]=NULL; //
      if (Serial1.available()) buffer[i]=(byte)Serial1.read();//
      };
    digitalWrite(led, HIGH);//
    if (buffer[0]==2){//
      for (int i=0;i<14;i++) {//
        Serial.print(buffer[i], HEX);
        Serial.print(":");
        };
      Serial.println("-OK");
      };
  while(Serial1.available()) Serial1.read();
  delay(2000);
  digitalWrite(led, LOW);
  };
}



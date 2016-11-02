#define Trig 13
#define Echo 12

int dist = 77;
int koof = 3;

int LampOut = 3;
volatile bool LampStat = false;
int DoorIn = 2;
volatile int d;

void Door(){
  //Serial.println("Start door");
  pinMode(DoorIn, INPUT);
  if (not(digitalRead(DoorIn))){
    LampStat = true;
    digitalWrite(LampOut, LampStat);
    //Serial.println("------------Lamp On");
    while (not(digitalRead(DoorIn))) {}
  }
  d = dist;
  attachInterrupt(0, Door, FALLING);
  //Serial.println("End Door");
}

int ReadDist(){
  //read distans
  unsigned int impulseTime=0; 
  unsigned int distance_sm=0;
  digitalWrite(Trig, HIGH); 
  delayMicroseconds(10); // равный 10 микросекундам 
  digitalWrite(Trig, LOW); // Отключаем 
  impulseTime=pulseIn(Echo, HIGH); // Замеряем длину импульса 
  distance_sm=impulseTime* 1.7 * 0.01;; // Пересчитываем в сантиметры 
  Serial.println(distance_sm);
  return distance_sm;
}

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT); //инициируем как выход 
  pinMode(Echo, INPUT); //инициируем как вход 
  pinMode(LampOut, OUTPUT);
  digitalWrite(LampOut, false);
  Door();
}

void loop() {
  delay(2000);
  if (LampStat){
    //Serial.println("--read dist");
    d = ReadDist();
    if (((dist + koof) > d) & ((dist - koof) < d)){
      LampStat = false;
      digitalWrite(LampOut, LampStat);
      //Serial.println("--lamp off");
    }
  }
}

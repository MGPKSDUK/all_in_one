#include <Servo.h>
Servo servo;
#include <AFMotor.h>
#define Echo A0
#define Trig A1
#define spoint 103
char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;
char hel;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

void setup() {
  Serial.begin(9600);
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
  servo.attach(10);
  M1.setSpeed(160);
  M2.setSpeed(160);
  M3.setSpeed(160);
  M4.setSpeed(160);
}
int obs = 0;
int blue = 0;
int v = 0;
void loop() {
  if(Serial.available() > 0){
    char command = Serial.read();
    if(command == "+"){
        Obstacle();
obs+=1;
    }
    if(command == "-"){
      Bluetooth();
      blue +=1;
    }
    if (command == "*"){
      voice();
      v+=1;
    }
  }
}

int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2; 
  return cm;
}

int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}

int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}

void Obstacle(){
for(;;){
distance = ultrasonic();
  if (distance <= 10) {
    Stop();
    backward();
    delay(100);
    Stop();
    L = leftsee();
    servo.write(spoint);
    delay(800);
    R = rightsee();
    servo.write(spoint);
    if (L < R) {
      right();
      delay(500);
      Stop();
      delay(200);
    } else if (L > R) {
      left();
      delay(500);
      Stop();
      delay(200);
    }
  } else {
    forward();
  }
  if(Serial.available()>0){
    hel=Serial.read();
  }
  if(hel == '+'){
    break;
  }
  else{
    continue;
  }
}
}

void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}
void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}
void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}

void voice() {
  for(;;){
    if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
    if (value == '1') {
      forward();
    } 
    else if (value == '2') {
      backward();
    } 
    else if (value == '3') {
      L = leftsee();
      servo.write(spoint);
      if (L >= 10 ) {
        left();
        delay(500);
        Stop();
      } else if (L < 10) {
        Stop();
      }
    } else if (value == '>') {
      R = rightsee();
      servo.write(spoint);
      if (R >= 10 ) {
        right();
        delay(500);
        Stop();
      } else if (R < 10) {
        Stop();
      }
    } else if (value == 'P') {
      Stop();
    }
  }
  if(Serial.available()>0){
    hel = Serial.read();
    // Serial.println(hel)
  }
  if(hel == '*'){
    break;
  }
  else{
    continue;
  }
  }
}

void Bluetooth() {
  for(;;){
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
  }
  if (value == 'F') {
    forward();
  } else if (value == 'B') {
    backward();
  } else if (value == 'L') {
    left();
  } else if (value == 'R') {
    right();
  } else if (value == 'S') {
    Stop();
  }
  if(Serial.available()>0){
    hel = Serial.read();
  }
  if(hel == '-'){
    break;
  }
  else{
    continue;
  }
}
}
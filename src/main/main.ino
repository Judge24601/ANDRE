#include <hcsr04.h>

// Libraries
#include <Arduino.h>
#include <Servo.h> 
#include <NewPing.h>

// Pins
const int rightTrigPin = 4;
const int rightEchoPin = 3;
const int trigPin = 12;
const int echoPin = 13;
const int servoPin = 2;
const int leftIRPin = 7;
const int rightIRPin = 8;
const int motorPin = 11;

// Constants
const int straightAngle = 35;
const int turnAngle = 30;
const int leftTurn = straightAngle - turnAngle;
const int rightTurn = straightAngle + turnAngle;
int maxDistance = 70;
int minDistance = 2;
const int turnTime = 250;
int minAvoidTime = 500;


// Initializations
NewPing sonar(trigPin, echoPin, 200);
NewPing rightSonar(rightTrigPin, rightEchoPin, 200);
Servo servo;
long initialTime;
bool turning = false;
int avoidStage;
int lastTurnDuration;
int correctionDuration;
int distance;

void setup() {
  // put your setup code here, to run once:
  servo.attach(servoPin);
  servo.write(straightAngle);
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);
  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 180);
  Serial.begin(9600);
  turning = false;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!turning) {
    if(checkDistance()) { 
      startTurn(leftTurn);
      turning = true;
      avoidStage = 0;
    }
  } else{
//    if (digitalRead(rightIRPin) != 0 && millis() - initialTime > turnTime && !checkDistance()){
//      turning = false;
//      analogWrite(motorPin, 180);
//      servo.write(straightAngle);
//    }
    Serial.print("avoidStage: ");
    Serial.println(avoidStage);
    switch(avoidStage){
      case 0:
        if(turnComplete(turnTime)){
          lastTurnDuration = millis() - initialTime;
          turnStraight();
          avoidStage = 1;
        }
        break;
      case 1: 
        if(turnComplete(minAvoidTime) && !checkRight()) {
          correctionDuration = millis() - initialTime;
          startTurn(rightTurn);
          avoidStage = 2;
        }
        break;
      case 2:
        if(turnComplete(lastTurnDuration)) {
          turnStraight();
          avoidStage = 3;
        }
        break;
      case 3:
        if(turnComplete(minAvoidTime) && !checkRight()) {
          startTurn(rightTurn);
          avoidStage = 4;
        }
        break;
      case 4:
        if(turnComplete(turnTime)) {
          turnStraight();
          avoidStage = 5;
        }
        break;
      case 5: 
        if(turnComplete(correctionDuration)){
          startTurn(leftTurn);
          avoidStage = 6;
        }
        break;
      case 6: 
        if(turnComplete(turnTime)) {
          turnStraight();
          turning = false;
          avoidStage = -1;
        }
        break;
    }
  }
  delay(5);
}

bool checkDistance () {
  distance = sonar.convert_cm(sonar.ping_median(5));
  return (distance < maxDistance && distance > minDistance);
}

bool checkRight () {
  distance = rightSonar.convert_cm(rightSonar.ping_median(5));
  return (distance < 10 && distance > minDistance);
}

bool turnComplete (int dur) {
  return millis() - initialTime >= dur;
}

void startTurn(int turn) {
  servo.write(turn);
  analogWrite(motorPin, 240);
  initialTime = millis();
}

void turnStraight() {
  servo.write(straightAngle);
  analogWrite(motorPin, 180);
  initialTime = millis();
}


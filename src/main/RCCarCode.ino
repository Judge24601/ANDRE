#include <hcsr04.h>

// Libraries
#include <Arduino.h>
#include <Servo.h> 
#include <NewPing.h>

// Pins
const int trigPin = 12;
const int echoPin = 13;
const int servoPin = 2;
const int leftIRPin = 7;
const int rightIRPin = 8;
const int motorPin = 11;

// Constants
const int straightAngle = 50;
const int turnAngle = 30;
const int leftTurn = straightAngle - turnAngle;
const int rightTurn = straightAngle + turnAngle;
int maxDistance = 70;
int minDistance = 2;
int turnTime = 500;


// Initializations
NewPing sonar(trigPin, echoPin, 200);
Servo servo;
long startTurnTime;
bool turning = false;

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
  
  Serial.println(distance);
  if (!turning) {
    if(checkDistance()) { 
      avoidObject();
    }
  } else{
    if (digitalRead(rightIRPin) != 0 && millis() - startTurnTime > turnTime && !checkDistance()){
      turning = false;
      analogWrite(motorPin, 180);
      servo.write(straightAngle);
    }
  }
  delay(50);
}

bool checkDistance () {
  distance = sonar.convert_cm(sonar.ping_median(5));
  return (distance < maxDistance && distance > minDistance);
}

void avoidObject() {
  servo.write(leftTurn);
  analogWrite(motorPin, 240);
  turning = true;
  startTurnTime = millis();
}


// #include <hcsr04.h>

// Libraries
#include <Arduino.h>
#include <Servo.h> 
#include <NewPing.h>
#include "Definitions.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <CmdMessenger.h> //I don't know if we need this yet
#include<MPU6050.h>
#include <math.h> 

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
const double straightAngle = 99.5;
const double turnAngle = 30;
const double leftTurn = straightAngle - turnAngle;
const double rightTurn = straightAngle + turnAngle + 5;
int maxDistance = 70;
int minDistance = 2;
const int turnTime = 300;
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

MPU6050 mpu;

unsigned long timer = 0;

float yaw = 0;
float forwardAccel = 0;
float accelOffset = 0.63;
float nextPointX;
float nextPointY;
float carCordX = 0;
float carCordY = 0;
float pathArray[3][2] = {{0,0},{0,1},{2,3}};
float oldVel = 0;
float newVel = 0;
float distance = 0;
float angleNeeded = 0;
float angleError = 0;
float oldError = 0;
float lastError = 0;
float wheelAngle = 0;

int angleDir = 0;
int timeStep = 175;
int rowNum;
int currentPointIndex = 0;

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
  while(!mpu.begin(MPU6050_SCALE_2000DPS,MPU6050_RANGE_2G)) {
    Serial.println("Alex, y u mess up the wirez");
  }
  mpu.calibrateGyro();
  mpu.setThreshold(1);
}

//void loop() {
//  // put your main code here, to run repeatedly:
//  if (!turning) {
//    if(checkDistance()) { 
//      startTurn(leftTurn);
//      turning = true;
//      avoidStage = 0;
//    }
//  } else{
////    if (digitalRead(rightIRPin) != 0 && millis() - initialTime > turnTime && !checkDistance()){
////      turning = false;
////      analogWrite(motorPin, 180);
////      servo.write(straightAngle);
////    }
//    Serial.print("avoidStage: ");
//    Serial.println(avoidStage);
//    switch(avoidStage){
//      case 0:
//        if(turnComplete(turnTime)){
//          lastTurnDuration = millis() - initialTime;
//          turnStraight();
//          avoidStage = 1;
//        }
//        break;
//      case 1: 
//        if(turnComplete(minAvoidTime) && !checkRight()) {
//          correctionDuration = millis() - initialTime;
//          startTurn(rightTurn);
//          avoidStage = 2;
//        }
//        break;
//      case 2:
//        if(turnComplete(lastTurnDuration + 50)) {
//          turnStraight();
//          avoidStage = 3;
//        }
//        break;
//      case 3:
//        if(turnComplete(minAvoidTime) && !checkRight()) {
//          startTurn(rightTurn);
//          avoidStage = 4;
//        }
//        break;
//      case 4:
//        if(turnComplete(turnTime + 50)) {
//          turnStraight();
//          avoidStage = 5;
//        }
//        break;
//      case 5: 
//        if(turnComplete(correctionDuration)){
//          startTurn(leftTurn);
//          avoidStage = 6;
//        }
//        break;
//      case 6: 
//        if(turnComplete(turnTime)) {
//          turnStraight();
//          turning = false;
//          avoidStage = -1;
//        }
//        break;
//    }
//  }
//}

void loop() {
  simplePID();
  //Serial.println(wheelAngle);
  //Serial.println(yaw);
  turnRef(-wheelAngle);
}

void turnRef(float angle) {
  int internalAngle = angle;
  servo.write(straightAngle + internalAngle);
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

/////////////////////////////
//HELPER FUNCTIONS
/////////////////////////////

//Finding the angle of the car
void carAngAcc() { 
  timer = millis();
  Vector normGyro = mpu.readNormalizeGyro();
  Vector normAccel = mpu.readNormalizeAccel();

  yaw = yaw + ((normGyro.ZAxis / 57296.00) * timeStep);

  forwardAccel = (normAccel.YAxis) + accelOffset;
 // sideAccel = (normAccel.XAxis);
  
 // Serial.print("Znorm : ");
 // Serial.println(yaw);
 // Serial.print(" | yAccel : ");
 // Serial.println(forwardAccel);
 // Serial.print(" | xAccel : ");
 // Serial.println(sideAccel);
 
  // Wait to full timeStep period
 // delay(30 - (millis() - timer));
}

//The first number in the communication string must be the number or coordinates
//void recieveCoordinates() {
 // int numOfCoordinates = 0;  
 // int counter = 0;
 // while (Serial.available() == 0);
 // rowNum = Serial.parseInt();
  //for (int i = 0; i < rowNum; i++){
  //  delay(1);
  //  pathArray[i] = new float[2];
  //  pathArray[i][0] = Serial.parseInt();
  //  pathArray[i][1] = Serial.parseInt();
//  }
//}

//Car position
void carLocation() {
  float tempTimeStep = timeStep / (float) 1000;
  newVel = oldVel + forwardAccel * tempTimeStep;
  distance = oldVel * tempTimeStep + (0.5 * forwardAccel * tempTimeStep * tempTimeStep);
  carCordY = carCordY + (cos(yaw) * distance);
  carCordX = carCordX + (sin(yaw) * distance);
  oldVel = newVel;
  Serial.println(distance);
}

void simplePID() {
  carAngAcc();
  carLocation();
  if (abs(carCordY - nextPointY) < 0.5 && abs(carCordX - nextPointX) < 0.5) {
    if (currentPointIndex < 3) {
      currentPointIndex ++;
    }
    nextPointX = pathArray[currentPointIndex][0];
    nextPointY = pathArray[currentPointIndex][1];
  }

  angleError = atan((nextPointY - carCordY)/(nextPointX - carCordX)) - yaw; 

  //wheelAngle = (Kp * angleError) + (Ki * oldError) + (Kd * (lastError - angleError));
  wheelAngle = Kp * angleError;
  
  if (wheelAngle < 0) {
    angleDir = -1;
  }
  else {
    angleDir = 1;
  }
  
  if ((maxTurn - (wheelAngle * angleDir)) < 0) {
     wheelAngle = maxTurn * angleDir;
  }
  
}

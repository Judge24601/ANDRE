#include "Definitions.h"
#include <SoftwareSerial.h>
#include<Wire.h>
#include <CmdMessenger.h> //I don't know if we need this yet
#include<Arduino.h>
#include<MPU6050.h>
#include <math.h> 

MPU6050 mpu;

unsigned long timer = 0;

float yaw = 0;
float forwardAccel = 0;
float nextPointX;
float nextPointY;
float carCordX = 0;
float carCordY = 0;
float *pathArray[2];
float oldVel = 0;
float newVel = 0;
float distance = 0;
float angleNeeded = 0;
float angleError = 0;
float oldError = 0;
float lastError = 0;
float wheelAngle = 0;

int angleDir = 0;
int timeStep = 50;
int rowNum;
int currentPointIndex = 0;

void setup(){
  Serial.begin(9600);
  while(!mpu.begin(MPU6050_SCALE_2000DPS,MPU6050_RANGE_2G)) {
    Serial.println("Alex, y u mess up the wirez");
  }
  mpu.calibrateGyro();
  mpu.setThreshold(3);
}

void loop() {
  carAngAcc();
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
  forwardAccel = (normAccel.YAxis);
 // sideAccel = (normAccel.XAxis);
  
 // Serial.print("Znorm : ");
 // Serial.println(yaw);
 // Serial.print(" | yAccel : ");
 // Serial.println(forwardAccel);
 // Serial.print(" | xAccel : ");
 // Serial.println(sideAccel);
 
  // Wait to full timeStep period
  delay(timeStep - (millis() - timer));
}

//The first number in the communication string must be the number or coordinates
void recieveCoordinates() {
  int numOfCoordinates = 0;  
  int counter = 0;
  while (Serial.available() == 0);
  rowNum = Serial.parseInt();
  for (int i = 0; i < rowNum; i++){
    delay(1);
    pathArray[i] = new float[2];
    pathArray[i][0] = Serial.parseInt();
    pathArray[i][1] = Serial.parseInt();
  }
}

//Car position
void carLocation() {
  newVel = oldVel + forwardAccel * timeStep;
  distance = oldVel * timeStep + (0.5 * forwardAccel * timeStep * timeStep);
  carCordY = carCordY + (cos(yaw) * distance);
  carCordX = carCordX + (sin(yaw) * distance);
  oldVel = newVel;
}

void simplePID() {
  carAngAcc();
  carLocation();
  currentPointIndex ++;
  nextPointX = pathArray[currentPointIndex][0];
  nextPointY = pathArray[currentPointIndex][1];

  angleError = atan((nextPointY - carCordY)/(nextPointX - carCordX)) - yaw;

  wheelAngle = (Kp * angleError) + (Ki * oldError) + (Kd * (lastError - angleError));

  if (wheelAngle < 0) {
    angleDir = -1;
  }
  else {
    angleDir = 1;
  }
  
  if (maxTurn - (wheelAngle * angleDir)) {
     wheelAngle = maxTurn * angleDir;
  }
  
}


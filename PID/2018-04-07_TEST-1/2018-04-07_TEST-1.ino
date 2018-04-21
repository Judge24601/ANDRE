#include "Definitions.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <CmdMessenger.h> //I don't know if we need this yet
#include <math.h> 

#include <Wire.h>
#include <MPU6050.h>
#include "AccelGyro.h"

SoftwareSerial bleSerial(rxPin,txPin);
MPU6050 mpu;
//Accelerometer accel;

float CTE; //Distance from the desired path
float CTERate; //How fast the car is moving in a direction perpendicular to the path
float CTESum; //Sums the cross track error to see if we spend more time on one side or the other
float steeringDelta = 0;
float *pathArray[2];

float yTheta = 0;
float yResting = 0;
float yAccel;

float xTheta = 0;
float xResting = 0;
float xAccel;

float carAccel;
float carAngle;
float offSetAngle;

int rowNum;
int currentPointIndex = 0;
float currentPointX;
float currentPointY;
float nextPointX;
float nextPointY;

float deltaX;
float deltaY;
float pathDist;
float pathAngle;

bool hasOffset = false;

void setup() {
  Serial.begin(115200);

  Serial.println(MPU6050_SCALE_2000DPS);
  Serial.println(MPU6050_RANGE_2G);

 // accel.makeItSo(mpu);
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
    Serial.println("NOT CONNECTED");
  }
  Serial.println("READY");
//  accel.update();
//  Serial.println(accel.get_xNorm());
////  Serial.println(accel.get_xNorm());
  //Serial.println(accel.get_xNorm());
  //Serial.println(accel.get_xNorm());
 // Serial.println(accel.get_xNorm());

//  zeroAccel();
}

void loop() {
 // accel.update();
  delay(100);
 // Serial.println(accel.get_xNorm());
//  Serial.println(mpu.readNormalizeAccel().XAxis);
  delay(1000);

}
/*
//HELPER FUNCTIONS

void accelUpdate() {
 // accel.update();
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

void steeringAngle() {
  steeringDelta = (pGain * CTE) + (dGain * CTERate) + (iGain * CTESum);
  if (steeringDelta > maxTurn) {
    steeringDelta = maxTurn;
  }
}

void pathDirVector() {
  currentPointX = pathArray[currentPointIndex][0];
  currentPointY = pathArray[currentPointIndex][1];
  currentPointIndex ++;
  nextPointX = pathArray[currentPointIndex][0];
  nextPointY = pathArray[currentPointIndex][1];

  deltaX = nextPointX - currentPointX;
  deltaY = nextPointY - currentPointY;
  
  pathDist = pow(deltaX,2) + pow(deltaY,2);
  pathDist = pow(pathAngle,0.5);

  pathAngle = tan(deltaY / deltaX);
}

void carDirVector() {
  xAccel = accel.get_xNorm() - xResting;
  xAccel = xAccel * cos(xTheta);

  yAccel = accel.get_yNorm() - yResting;
  yAccel = yAccel * cos(yTheta);

  carAccel = pow(xAccel,2) + pow(yAccel, 2);
  carAccel = pow(carAccel,0.5);

  carAngle = tan(yAccel / xAccel);
  offSetAngle += carAngle;
}

void zeroAccel() {
  yResting = accel.get_yNorm();
  yTheta = acos((accel.get_yNorm() / 9.8)); 
  yTheta = (M_PI/2) - yTheta;
  
  xResting = accel.get_xNorm();
  xTheta = acos((accel.get_xNorm() / 9.8));
  xTheta = (M_PI/2) - xTheta;

  if (yTheta || xTheta) {
    hasOffset = true;
  }
  
}
*/

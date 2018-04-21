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
int timeStep = 10;

void setup(){
  Serial.begin(9600);
  while(!mpu.begin(MPU6050_SCALE_2000DPS,MPU6050_RANGE_2G)) {
    Serial.println("AHHH");
  }
  mpu.calibrateGyro();
  mpu.setThreshold(3);
}

void loop() {
  timer = millis();
  Vector normGyro = mpu.readNormalizeGyro();

  yaw = yaw + normGyro.ZAxis * timeStep;
  
  Serial.print("Znorm : ");
  Serial.println(yaw);
  
  
  // Wait to full timeStep period
  delay((timeStep*1000) - (millis() - timer));
} 



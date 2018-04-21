#include <Wire.h>
#include <MPU6050.h>
#include <Arduino.h>
#include "AccelGyro.h"
// Serial.begin(115200);

  //Serial.println("Initialize MPU6050");
//constructor
Accelerometer::Accelerometer(MPU6050 _mpu) {
  
  mpu = _mpu;
  int counter = 0;
	while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
	//while (true) {
		counter ++;
   if (counter > 6) break;
  }
}
Accelerometer::Accelerometer() {
  delay(10);
}
//methods

void Accelerometer::makeItSo(MPU6050 _mpu) {
  mpu = _mpu;
  int counter = 0;
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
  //while (true) {
    counter ++;
   if (counter > 6) break;
  }
}

void Accelerometer::update() {
	rawAccel = mpu.readRawAccel();
	normAccel = mpu.readNormalizeAccel();

  _xRaw = rawAccel.XAxis;
  _yRaw = rawAccel.YAxis;
  _zRaw = rawAccel.ZAxis;

  _xNorm = normAccel.XAxis;
  _yNorm = normAccel.YAxis;
  _zNorm = normAccel.ZAxis;
/*
  _xOffset = mpu.getAccelOffsetX();
  _yOffset = mpu.getAccelOffsetY();
  _zOffset = mpu.getAccelOffsetZ();
*/
  
}

/////////////////// DATA GETTERS //////////////////////

//X
float Accelerometer::get_xRaw() {
	return _xRaw;
}
float Accelerometer::get_xNorm() {
	return _xNorm;
}
//float Accelerometer::get_xOffset() {
//	return _xOffset;
///}

// Y
float Accelerometer::get_yRaw() {
	return _yRaw;
}
float Accelerometer::get_yNorm() {
	return _yNorm;
}
//float Accelerometer::get_yOffset() {
//	return _yOffset;
//}

//Z
float Accelerometer::get_zRaw() {
	return _zRaw;
}
float Accelerometer::get_zNorm() {
	return _zNorm;
}
//float Accelerometer::get_zOffset() {
//	return _zOffset;
//}
/*
//////////////////// MISC GETTERS /////////////////////
void Accelerometer::get_clockSource() {
	Serial.println(mpu.getClockSource());
}
void Accelerometer::get_sleepMode() {
	Serial.println(mpu.getSleepEnabled());
}
void Accelerometer::get_range() {
	Serial.println(mpu.getRange());
}*/

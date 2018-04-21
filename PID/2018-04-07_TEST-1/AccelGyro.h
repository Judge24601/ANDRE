#ifndef AccelGyro_H
#define AccelGyro_H

#include <Wire.h> 
#include <MPU6050.h>
#include <Arduino.h>

class Accelerometer
{
public:
	//constructor
	Accelerometer(MPU6050 _mpu);
  Accelerometer();
  
	//methods
  void makeItSo(MPU6050 _mpu);
	void update();

	float get_xRaw();
	float get_xNorm();
	float get_xOffset();

	float get_yRaw();
	float get_yNorm();
	float get_yOffset();

	float get_zRaw();
	float get_zNorm();
	float get_zOffset();
/*
	void get_clockSource();
	void get_sleepMode();
	void get_range();
*/
private:
  MPU6050 mpu;
	//data
	Vector rawAccel;
	Vector normAccel;
	float _xRaw;
	float _xNorm;
//	float _xOffset;
  float _yRaw;
	float _yNorm;
//	float _yOffset;
	float _zRaw;
	float _zNorm;
//	float _zOffset;
};
#endif

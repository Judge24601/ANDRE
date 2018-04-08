#include "Definitions.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <CmdMessenger.h> //I don't know if we need this yet

SoftwareSerial bleSerial(rxPin,txPin);

float CTE; //Distance from the desired path
float CTERate; //How fast the car is moving in a direction perpendicular to the path
float CTESum; //Sums the cross track error to see if we spend more time on one side or the other
float steeringDelta = 0;
float *pathArray[2];

int rowNum;

void setup() {
  Serial.begin(9600);
}

void loop() {
}

//HELPER FUNCTIONS

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
  
}


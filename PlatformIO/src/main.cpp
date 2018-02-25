/*
Into Robotics
*/
 
#include <servo.h>  //add '<' and '>' before and after servo.h
#include <Arduino.h>
// #include <PulseInZero.h>  // try Github repo to pulsein without delay

#define LEDPin 13 // Onboard LED
int receiver_pin = 8;
int servoPin = 2;

bool range = false;
bool turned = false;

// defines pins numbers
const int motorPin = 7;
const int trigPin = 9;
const int echoPin = 10;
// defines variables
int maximumRange = 30; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
bool motorOn = false; 
Servo servo;  
int motorCounter = 0;
int servoAngle = 0;   // servo position in degrees
 
void setup()
{
  Serial.begin(9600);  
  servo.attach(servoPin);
  pinMode(motorPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
  
  // set up PulseInZero, pass in callback function to be triggered
  // PulseInZero::setup(pulseInComplete);
  // PulseInZero::begin();
}

void motorPWM(int delayOff, int delayOn) {
  if (motorOn && motorCounter > delayOn) {
    digitalWrite(motorPin, LOW);
    motorCounter = 0;
    motorOn = false;
  } else if (!motorOn && motorCounter > delayOff) {
    digitalWrite(motorPin, HIGH);
    motorCounter = 0;
    motorOn = true;
  }
  
} 
 
void loop()
{
  motorPWM(40, 10); 
//85 is default

  servo.write(85);

 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH, 1000);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (digitalRead(receiver_pin)==LOW ){
  /* Send a negative number to computer and Turn LED ON 
  to indicate "out of range" */
  digitalWrite(LEDPin, HIGH); 
  if (!turned){
    servo.write(85);
    //  delay(1000);
    turned = true;
  }
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
  Serial.println(distance);
  digitalWrite(LEDPin, LOW); 
    servo.write(70);
    //  delay(1000);
   turned = false;
 }
 if(motorCounter %2 == 0) { 
   digitalWrite(LEDPin, LOW); 
  } else {
    digitalWrite(LEDPin, HIGH);
  }
 //Delay 50ms before next reading.
  motorCounter++;
}

/**
* Pulse complete callback hanlder for PulseInZero
* @param duration - pulse length in microseconds
*/
// void pulseInComplete(unsigned long duration){
//   // note: if you're detecting a lot of pulses a second it's probably best to remove the serial prints... 
//   // (this function is triggered by an interrupt so the serial buffer can fill up and cause the program to hang)
//   Serial.print("pulse complete - duration: ");
//   Serial.print(duration);
//   Serial.println(" us");
  
//   PulseInZero::begin();  // Start listening again...  
  
// }

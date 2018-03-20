/*
 * ANDRE Main code logic.
 */

#include <servo.h> 
#include <Arduino.h>
// #include <PulseInZero.h>  // try Github repo to pulsein without delay

bool turned = false;

// Define pins numbers
const int motorPin = 7;
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 2;
const int IRPinLeft = 5;
const int IRPinRight = 6;

// Other constants
const int defaultServoAngle = 65; // Reference angle in which the wheels are aligned straight
const int bitToMotorRPM = 0; // Conversion factor from an 8-bit number (0 to 255) to some speed of the motor
const int maxServoAngle = 0; // Maximum angle from straight in which the servo can comfortably rotate.

// Global variables to store data
int maximumRange = 30; // Maximum range threshold for sonar
int minimumRange = 0; // Minimum range threshold for sonar
long duration, distance; // Duration used to calculate distance
Servo servo;
bool motorOn = false;
int motorCounter = 0;
int servoAngle = 0; // servo position in degrees

// Function prototypes
void turnAbs(int angle);
void turnRef(int angle);
void turnStraignt();
void getSonarData();
void changeMotorSpeed(int RPM);

// Initialize the Arduino
void setup() {
  Serial.begin(9600); // Output serial data specifier
  servo.attach(servoPin);

  pinMode(motorPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // set up PulseInZero, pass in callback function to be triggered
  // PulseInZero::setup(pulseInComplete);
  // PulseInZero::begin();
}

// Logic that runs continuosly
void loop() {

  getSonarData();

  if (distance < maximumRange) {
    if (!turned) {
      // We've encountered an object. Turn right and slow down.
      turnRef(12);
      changeMotorSpeed(100);
      delay(100); // Delay a certain period of time for the car to complete a 90-degree turn.

      // We've completed an turn. Drive straight and keep straight until the left IR pin no longer detects the object.
      turnStraight();
      while(digitalRead(IRPinLeft) == HIGH) delay(10);

      // We've avoided the object. Return to default state.
      turnRef(-12);
      changeMotorSpeed(400);
    }
  } else {
    turnStraight();
  }
}

/*
 * Turns the servo a certain angle away from the straight angle.
 */
void turnRef(int angle){
  servo.write(defaultServoAngle + angle);
  turned = true;
}

/*
 * Turns the servo some absolute angle
 */ 
void turnAbs(int angle){
  servo.write(angle);
  turned = true;
}

/*
 * Turns the servo to its default straight position.
 */ 
void turnStraight() {
  servo.write(defaultServoAngle);
  turned = false;
}

/*
 * Changes the motor speed from an input RPM.
 */ 
void changeMotorSpeed(int rpm){
  int speed8bit = rpm / bitToMotorRPM;
  // Cannot have values greater than 255 or less than 0.
  if (speed8bit > 255) speed8bit = 255;
  if (speed8bit < 0) speed8bit = 0;
  analogWrite(motorPin, speed8bit);
}

/**
 * Pules a wave in the sonar and retrieves the distance.
 */
void getSonarData() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 1000);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;
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
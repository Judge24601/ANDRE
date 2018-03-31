/*
Into Robotics
*/
 
#include <Servo.h>  //add '<' and '>' before and after servo.h
#include <Arduino.h>
#define LEDPin 13 // Onboard LED
 
int servoPin = 2;

bool range = false;
bool turned = false;

// defines pins numbers
const int trigPin = 13;
const int echoPin = 12;
// defines variables
int maximumRange = 40; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
 
Servo servo;  
 
int servoAngle = 0;   // servo position in degrees
 
void setup()
{
  Serial.begin(9600);  
  servo.attach(servoPin);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
 servo.write(85);
}
 
 
void loop()
{
//85 is default
 analogWrite(3, 255);
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
  /* Send a negative number to computer and Turn LED ON 
  to indicate "out of range" */
  Serial.println("-1");
  digitalWrite(LEDPin, HIGH); 
  if (!turned){
    servo.write(65);
    //  delay(1000);
    turned = true;
  }
 }
 else {
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
  Serial.println(distance);
  digitalWrite(LEDPin, LOW); 
    servo.write(85);
    //  delay(1000);
   turned = false;
 }
 
 //Delay 50ms before next reading.
  delay(50);

}


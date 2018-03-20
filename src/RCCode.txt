""" RC car code"""

objectAvoidanceDistance = 100;

rightV = 0;
leftV = 255;
straightV = (leftV + rightV)/2;

#Front IR Sensor
int frontSensorPin = 0;

#Side IR Sensor
int leftSensorPin = 1;
int rightSensorPin = 2;

#Set servo pin
int servoPin = 3;
pinMode(servoPin, OUTPUT);

#Set main motor to high
int driveTrainPin = 0;
pinMode(driveTrainPin, OUTPUT);

#start running superloop
while(1) {

    #Read front IR sensor
    dist = analogRead(frontSensorPin);

    if(dist > objectAvoidanceDistance && dist != -1) {
        avoid();
    }
}

void avoid() {
#Turn front wheels right
    analogWrite(servoPin, rightV);
    
    """Perform 90deg turn"""
    
#Turn front wheels to straight
    analogWrite(servoPin, straightV);
    
#Keep straight while side sensor reads high voltage
    while(analogRead(leftSensorPin) > clearDistance) {
#Keep going forwards
        
        """After side is clear"""
    }
    
#Turn front wheels left
    analogWrite(servoPin, leftV);
    
    """Perform 90deg turn"""
    
#Turn front wheels straight
    analogWrite(servoPin, straightV);
    
    """Car now parallel to object, headed in correct direction"""
    
#Keep reading same side sensor
    while(GPIO.input(2) > clearDistance){
        
    }
    """After object has been passed, turn back on to correct path"""
    
#Turn front wheels left
    analogWrite(servoPin, leftV);
    
#Go some correction distance
    
#Turn front wheels right, back on to correct path
    analogWrite(servoPin, rightV);
    
    """Perform 90deg turn"""
    
#Set front wheels straight
    analogWrite(servoPin, straightV);
    
#end avoid() method
}

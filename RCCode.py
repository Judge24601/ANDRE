""" RC car code"""
import RPi.GPIO as GPIO

objectAvoidanceDistance = 3

rightV = GPIO.HIGH
leftV = GPIO.LOW
straightV = (leftV + rightV)/2

GPIO.setmode(GPIO.BOARD)

#Front IR Sensor
GPIO.setup(1, GPIO.IN)

#Side IR Sensor
GPIO.setup(2, GPIO.IN)

GPIO.setup(0, GPIO.OUT, initial = straightV)

#Set main motor to high
GPIO.setup(18, GPIO.OUT, initial = GPIO.HIGH)

#start running superloop
while(1):

    #Read front IR sensor
    dist = GPIO.input(1)
    print dist
    if(dist > objectAvoidanceDistance):
    #Run object avoidance method
        avoid()

def avoid():
    #Turn front wheels right
    GPIO.output(0, rightV)

    """Perform 90deg turn"""

    #Turn front wheels to straight
    GPIO.output(0, straightV)

    #Keep straight while side sensor reads high voltage
    while(GPIO.input(2) > clearDistance):
    #Keep going forwards

"""After side is clear"""

#Turn front wheels left
GPIO.output(0, leftV)

"""Perform 90deg turn"""

#Turn front wheels straight
GPIO.output(0, straightV)

"""Car now parallel to object, headed in correct direction"""

#Keep reading same side sensor
    while(GPIO.input(2) > clearDistance):

"""After object has been passed, turn back on to correct path"""

#Turn front wheels left
GPIO.output(0, leftV)

#Go some correction distance

#Turn front wheels right, back on to correct path
GPIO.output(0, rightV)

"""Perform 90deg turn"""

#Set front wheels straight
GPIO.output(0, straightV)

#end avoid() method

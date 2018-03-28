/*/*
Into Robotics
*/
#include <Arduino.h>
#include <SoftwareSerial.h>
// Download the CmdMessenger.h library and follow the instructions on its git page
#include <CmdMessenger.h>

#define RXPIN 2
#define TXPIN 3


SoftwareSerial bleSerial(TXPIN,RXPIN);
CmdMessenger bleMessenger(bleSerial);

//Serial communication can only occur as characters as far as I know (at least if you want to use arduino libraries)
char j = 0;

void setup () {
  Serial.begin(9600);
  bleSerial.begin(9600);


  //Initialize the blue tooth module. The commands can be found at https://github.com/yostane/arduino-at-09/blob/master/hm-10-datasheet.pdf
  sendCommand("AT");
  sendCommand("AT+ROLE0");
  sendCommand("AT+UUID0xFFE0");
  sendCommand("AT+CHAR0xFFE1");
  sendCommand("AT+NAMEbluino");

  //For the IR sensor
  pinMode(A0, INPUT);
}

//HELPER FUNCTIONS

//Serial is the arduino serial port, and bleSerial is a serial communication port to the bleModule
void sendCommand(const char * command){
  Serial.print("Command send :");
  Serial.println(command);
  bleSerial.println(command);
  //wait some time
  delay(100);
  
  char reply[100];
  int i = 0;
  while (bleSerial.available()) {
    reply[i] = bleSerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  Serial.print(reply);
  Serial.println("Reply end");
}


void readSerial(){
  char reply[50];
  int i = 0;
  while (bleSerial.available()) {
    reply[i] = bleSerial.read();
    i += 1;
  }
  //end the string
  reply[i] = '\0';
  if(strlen(reply) > 0){
    Serial.print(reply);
    Serial.println("We have just read some data");
  }
}

void writeToBLE(char value) {
  Serial.print("Writing hex :");
  Serial.println(value, HEX);
  bleSerial.write(value);
}


void loop () {
  writeToBLE(j);
  j = analogRead(0);
  delay(500);
}




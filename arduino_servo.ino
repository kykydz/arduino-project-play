/*
This project use NodeMCu ESP8266 Generic Module
*/

// lib
#include <Servo.h>

// define module lib
Servo myServo;

// define pin I/O
const int analogPotensioPin = A0;
const int triggerPin = 4;// D2
const int echoPin = 5;// D1

// define global variable as memory
int currPotensioValue = 0;
int prevPotensioValue = 0;
int mappedServoAngle = 0;

void setup() {
  Serial.begin(9600);
  myServo.attach(2);
  pinMode(analogPotensioPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // read potensio value
  currPotensioValue = analogRead(analogPotensioPin);
  Serial.print("prev pot val = " + String(currPotensioValue) + " |  current pot val = " + String(currPotensioValue));
  Serial.println("| Servo Angle = " + String(mappedServoAngle));
  
  int diffAngle = abs(prevPotensioValue - currPotensioValue);
  if (diffAngle > 15) {
    doServoThings(currPotensioValue, diffAngle);
  }

  prevPotensioValue = currPotensioValue;
}

void doServoThings(int potensioValue, int diffAngle){
  // raw mapping potensio
  // mappedServoAngle = map(potensioValue, 0, 1023, 0, 180);

  // advanced mapping potensio to avoid deadzone servo (if any)
  mappedServoAngle = mapServoAvoidDeadZone(potensioValue);
  Serial.println("mapped Value = " + String(mappedServoAngle));

  myServo.write(mappedServoAngle);
  int defaultDelay = 25;
  delay(defaultDelay);
}

int mapServoAvoidDeadZone(int potensioValue) {
  int deadZone = 10; // Adjust based servo's behavior
  int potensioCenter = 512; // Assuming mid-point for 0-1023 range

  int mappedServoAngle;
  if (potensioValue < potensioCenter + deadZone && potensioValue > potensioCenter - deadZone) {
    mappedServoAngle = 90; // Set a specific center angle (adjust if needed)
  } else {
    mappedServoAngle = map(potensioValue, 0, 1023, 0, 180);
  }

  return mappedServoAngle;
};

// Below code is not working yet
// sensor: HY-SRF05
void doCalculateDistance() {
  const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
  int distance= duration/29/2;
  if(duration==0){
    Serial.println("Warning: no pulse from sensor");
  } else {
    Serial.print("distance to nearest object:");
    Serial.println(distance);
    Serial.println(" cm");
  }
  delay(100);
}

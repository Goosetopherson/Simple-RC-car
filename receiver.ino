/*
  Author: Christopher Guzman
  Date: 6/16/2024
  Description: Code to receive transmission for RC CSUSB Cart from controller transmitter.
  Code Receives two values from controller (X, Y). The X value will control the steering while Y
  value will control forward or backward speed.
*/

#include <SPI.h>
#include "NRFLite.h"
#include <Servo.h>
#include <L298N.h>

// NRF module instance
NRFLite _radio;

// Servo instance
Servo servo;

// array to hold the joystick values.
int _data[2];

// PIN DEFINITIONS
//NRF24L01+ pins
#define CE 2
#define CSN 3
// Motor Pins
#define ENA 6
#define IN1 7
#define IN2 8

// Motor instance
L298N motor(ENA, IN1, IN2);

// values for _angle and _speed
int _speed = 0;
int _angle = 0;
int center = 50;  // specific to this servo. All servos must be checked for center

// flag to check which direction to travel
int direction = 0;

// function sets the values for speed and angle from received data
void setValues(int x, int y);
void printValues();
void moveMotor();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // initialize NRF module
  _radio.init(0, CE, CSN);

  //motor speed initialized
  motor.setSpeed(_speed);

  // attach servo
  servo.attach(5);
  servo.write(center);
}

void loop() {

  while (_radio.hasData()) {
    _radio.readData(&_data);

    setValues(_data[0], _data[1]);
    moveMotor();
    printValues();
    
    servo.write(_angle);
  }
}

void setValues(int y, int x) {
  // check speed and direction
  if (y <= 500) {
    direction = 2;
    _speed = map(abs(y - 500), 0, 500, 0, 254);
  }
  else if (y >= 510) {
    direction = 1;
    _speed = map(y, 500, 1023, 0, 254);
  }
  else {
    direction = 0;
    _speed = 0;
  }

  // check the angle for servo
  if (x <= 500) {
    _angle = 180;
  }
  else if (x >= 510) {
    _angle = 0;
  } 
  else {
    _angle = center;
  }
}

void moveMotor() {
  motor.setSpeed(_speed);
  if (direction == 1) {
    motor.forward();
  } else if (direction == 2) {
    motor.backward();
  } else {
    motor.stop();
  }
}

void printValues() {
  // print out values for debugging
  Serial.print("X : ");
  Serial.print(_angle);
  Serial.print(", Y : ");
  Serial.print(_speed);
  if (direction == 1) {
    Serial.println(" -- Moving Forward.");
  } else if (direction == 2) {
    Serial.println(" -- Moving Backward.");
  } else {
    Serial.println(" -- Motor Stopped");
  }
  Serial.println(motor.getSpeed());
}

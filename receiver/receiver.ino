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
#include <EEPROM.h>

// NRF module instance
NRFLite _radio; // pins(MOSI - D11, VCC - 3.3V, MISO - D12, SCK - D13, CSN - D2, CE - D3, GND)

// Servo instance
Servo servo;

// array to hold the joystick values.
int _data[5];

// PIN DEFINITIONS
//NRF24L01+ pins
#define CE 9
#define CSN 10

// Motor Pins
#define ENA 6
#define IN1 7
#define IN2 8

#define ENA_2 3
#define IN1_2 4
#define IN2_2 5

// Motor instance
L298N motor(ENA, IN1, IN2);
L298N motor2(ENA_2, IN1_2, IN2_2);

// values for _angle and _speed
int _speed = 0;
int _angle = 0;
int centerAdd = 0;
int center = 0;  // specific to this servo. All servos must be checked for center

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
  motor2.setSpeed(_speed);

  // check if we have previously stored a value for servo's center
  if(EEPROM.read(centerAdd) >= 0 && EEPROM.read(centerAdd) <= 180){
    center = EEPROM.read(centerAdd);
  }
  else{
    center = 90;
  }

  // attach servo
  servo.attach(2);
  servo.write(center);
}

void loop() {

  while (_radio.hasData()) {
    _radio.readData(&_data);

    if(_data[2] == 0){
      if(_data[3] == 0){
        center++;
      }
      if(_data[4] == 0){
        center--;
      }

      Serial.println(center);
      delay(100);
    }

    setValues(_data[0], _data[1]);
    moveMotor();
    //printValues();
    
    servo.write(_angle);
    //printValues();
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
   // _angle = map(abs(x - 500), 0, 500, 0, 90);
    _angle = 180;
  }
  else if (x >= 510) {
    //_angle = map(x, 500, 1023, 90, 180);
    _angle = 0;
  } 
  else {
    _angle = center;
  }
}

void moveMotor() {
  motor.setSpeed(_speed);
  motor2.setSpeed(_speed);
  if (direction == 1) {
    motor.forward();
    motor2.forward();
  } else if (direction == 2) {
    motor.backward();
    motor2.backward();
  } else {
    motor.stop();
    motor2.stop();
  }
}

void printValues() {
  // print out values for debugging
  Serial.print("Angle : ");
  Serial.print(_angle);
  Serial.print(", Speed : ");
  Serial.print(_speed);
  if (direction == 1) {
    Serial.println(" -- Moving Forward.");
  } else if (direction == 2) {
    Serial.println(" -- Moving Backward.");
  } else {
    Serial.println(" -- Motor Stopped");
  }
  //Serial.println(motor.getSpeed());
}

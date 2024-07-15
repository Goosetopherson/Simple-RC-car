/*
  Author: Christopher Guzman
  Date: 6/16/2024
  Description: Code to transmit for RC CSUSB Cart from controller to receiver.
  Code transmits two values from controller (X, Y). The X value will control the steering while Y
  value will control forward or backward speed.
*/

#include <SPI.h>
#include "NRFLite.h"

// NRF module instance
NRFLite _radio;

// array to hold the joystick values
int _data[5];

// PIN DEFINITIONS
// NRF module pins
#define CE 9
#define CSN 10
// joystick pins
#define YAXIS A0 // VRX on joystick
#define XAXIS A1 // VRY on joystick

// trim pins
#define TRIM 6
#define T_ADD 7
#define T_SUB 8

void printData();

void setup()
{

  Serial.begin(115200);
  // initialize NRF module
  _radio.init(1, CE, CSN); // Set radio to Id = 1, along with the CE and CSN pins
  // initialize pins for joystick axes
  pinMode(YAXIS, INPUT);
  pinMode(XAXIS, INPUT);

  //Initialize pins for trim
  pinMode(TRIM, INPUT_PULLUP);
  pinMode(T_ADD, INPUT_PULLUP);
  pinMode(T_SUB, INPUT_PULLUP);
}

void loop()
{
  //store values in data array to transmit
  _data[0] = analogRead(YAXIS);
  _data[1] = analogRead(XAXIS);
  _data[2] = digitalRead(TRIM);
  _data[3] = digitalRead(T_ADD);
  _data[4] = digitalRead(T_SUB);
  printData();
  _radio.send(0, &_data, sizeof(_data)); // Send data to the radio with Id = 0
  delay(10);
}

void printData(){
  Serial.print("Speed : ");
  Serial.print(_data[0]);
  Serial.print(", Angle : ");
  Serial.println(_data[1]);
  Serial.print("TRIM: ");
  Serial.print(_data[2]);
  Serial.print(" : ADD: ");
  Serial.print(_data[3]);
  Serial.print(" : SUB: ");
  Serial.println(_data[4]);
}
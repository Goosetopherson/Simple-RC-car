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
int _data[2];

// PIN DEFINITIONS
// NRF module pins
#define CE 2
#define CSN 3
// joystick pins
#define YAXIS A0 // VRX on joystick
#define XAXIS A1 // VRY on joystick

void setup()
{
  // initialize NRF module
  _radio.init(1, CE, CSN); // Set radio to Id = 1, along with the CE and CSN pins
  // initialize pins for joystick axes
  pinMode(YAXIS, INPUT);
  pinMode(XAXIS, INPUT);
}

void loop()
{
  //store values in data array to transmit
  _data[0] = analogRead(YAXIS);
  _data[1] = analogRead(XAXIS);
  _radio.send(0, &_data, sizeof(_data)); // Send data to the radio with Id = 0
  delay(10);
}
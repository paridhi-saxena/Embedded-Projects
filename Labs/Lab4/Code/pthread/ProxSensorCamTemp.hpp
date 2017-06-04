/****************************************************************
ProximityTest.ino
APDS-9960 RGB and Gesture Sensor
Shawn Hymel @ SparkFun Electronics
October 28, 2014
https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor
Tests the proximity sensing abilities of the APDS-9960.
Configures the APDS-9960 over I2C and polls for the distance to
the object nearest the sensor.
Hardware Connections:
IMPORTANT: The APDS-9960 can only accept 3.3V!
 
 Arduino Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
Resources:
Include Wire.h and SparkFun_APDS-9960.h
Development environment specifics:
Written in Arduino 1.0.5
Tested with SparkFun Arduino Pro Mini 3.3V
Distributed as-is; no warranty is given.
****************************************************************/
#include "SparkFun_APDS9960.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include <unistd.h>
#include <iostream>
#include <stdio.h>

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;

using namespace std;
using namespace cv;

extern "C" {
	#include "temp.c"
	float getTemp();
}
int initSensor() {
  
  // Initialize Serial port
  cout <<"------------------------------------"<<endl;
  cout <<"SparkFun APDS-9960 - ProximitySensor"<<endl;
  cout <<"------------------------------------"<<endl;
  
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    cout <<"APDS-9960 initialization complete"<<endl;
  } else {
    cout <<"Something went wrong during APDS-9960 init!"<<endl;
  }
  
  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_1X) ) {
    cout <<"Something went wrong trying to set PGAIN"<<endl;
  }
  
  // Start running the APDS-9960 proximity sensor (no interrupts)
  if ( apds.enableProximitySensor(false) ) {
    cout <<"Proximity sensor is now running"<<endl;
  } else {
    cout <<"Something went wrong during sensor init!"<<endl;
  }
}

int getProxVal() {
  // Read the proximity value
  if ( !apds.readProximity(proximity_data) ) {
    cout <<"Error reading proximity value"<<endl;
  } else {
    printf("Proximity: %d\n", proximity_data);
  }
  // Wait 250 ms before next reading
  usleep(1*1000000);
  if(proximity_data > 150) {
    	
  }
  return proximity_data;
 }

float getTempVal() {
  float temp = getTemp();
  if(temp > 26) {
  }
  return temp;
}

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
int mainofP() {
  
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
  
  // Read the proximity value
  int i = 0;
  for(i=0;i<50;i++) {
  if ( !apds.readProximity(proximity_data) ) {
    cout <<"Error reading proximity value"<<endl;
  } else {
    cout <<"Proximity: "<<endl;
    printf("%d\n", proximity_data);
  }
  // Wait 250 ms before next reading
  usleep(1*1000000);
  if(proximity_data > 150) {
    	CvCapture* capture = 0;
	Mat frame, frameCopy, image;
	capture = cvCaptureFromCAM(CV_CAP_ANY); //0=default, -1=any camera, 1..99=your camera
	if(!capture) {
		cout << "No camera detected" << endl;
	}

	//cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
	int i;
	if(capture) {
		cout << "In capture ..." << endl;
		for(i = 0;i < 1; i++) {
			IplImage* iplImg = cvQueryFrame(capture);
			frame = iplImg;
			cout << "after img" << endl;
			if(frame.empty())
				break;
			if(iplImg->origin == IPL_ORIGIN_TL)
				frame.copyTo(frameCopy);
			else
				flip(frame, frameCopy, 0);

			//cvShowImage("result", iplImg);
			cout << "before save" << endl;
			//if(waitKey(1) >= 0) {
				vector<int> cp;
				cp.push_back(CV_IMWRITE_PNG_COMPRESSION);
				cp.push_back(3);
				imwrite("pic.jpg", frame, cp);
				cout << "image saved" << endl;
				break;
			//}
		}
		// waitKey(0);
	}

	cvReleaseCapture(&capture);
	//cvDestroyWindow("result");

	break;
  }
  }
  float temp = getTemp();
  if(temp > 26) {
    	CvCapture* capture = 0;
	Mat frame, frameCopy, image;
	capture = cvCaptureFromCAM(CV_CAP_ANY); //0=default, -1=any camera, 1..99=your camera
	if(!capture) {
		cout << "No camera detected" << endl;
	}

	//cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
	int i;
	if(capture) {
		cout << "In capture ..." << endl;
		for(i = 0;i < 1; i++) {
			IplImage* iplImg = cvQueryFrame(capture);
			frame = iplImg;
			cout << "after img" << endl;
			if(frame.empty())
				break;
			if(iplImg->origin == IPL_ORIGIN_TL)
				frame.copyTo(frameCopy);
			else
				flip(frame, frameCopy, 0);

			//cvShowImage("result", iplImg);
			cout << "before save" << endl;
			//if(waitKey(1) >= 0) {
				vector<int> cp;
				cp.push_back(CV_IMWRITE_PNG_COMPRESSION);
				cp.push_back(3);
				imwrite("pic.jpg", frame, cp);
				cout << "image saved" << endl;
				break;
			//}
		}
		// waitKey(0);
	}

	cvReleaseCapture(&capture);
	//cvDestroyWindow("result");

	return 0;
}
}

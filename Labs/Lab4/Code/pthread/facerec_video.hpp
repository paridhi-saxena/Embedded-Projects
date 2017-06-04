/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef __cplusplus
extern "C"
#endif
int captureAndProcess();
using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

int captureAndProcess() {
    /*string fn_haar = string(argv[1]);
    string fn_csv = string(argv[2]);
    int deviceId = atoi(argv[3]);*/
    FILE *fp = fopen("face_recognizer.jpg", "w+");
    int faceFound = 0;
    string fn_haar = "haarcascade_frontalface_default.xml";
    string fn_csv = "csv.txt";
    int deviceId = -1;
    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        exit(1);
    }
    cout<<"After csv file read"<<endl;
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    // Create a FaceRecognizer and train it on the given images:
    cout<<"Before trainig"<<endl;
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(images, labels);
    cout<<"after training"<<endl;
    CascadeClassifier haar_cascade;
    cout<<"before haarcascade load"<<endl;
    haar_cascade.load(fn_haar);
    // Get a handle to the Video device:
    cout<<"Before device capture"<<endl;
    VideoCapture cap(deviceId);
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    // Holds the current frame from the Video device:
    Mat frame;
    int i;
    for(i =0;i<1;i++) {
        cap >> frame;
        // Clone the current frame:
        Mat original = frame.clone();
	cout<<"After capture"<<endl;
        // Convert the current frame to grayscale:
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);
        cout<<faces.size()<<endl;
	if(faces.size() > 0) {
		faceFound = 1;
	}
	for(int i = 0; i < faces.size(); i++) {
            // Process face by face:
            Rect face_i = faces[i];
            // Crop the face from the image. So simple with OpenCV C++:
            Mat face = gray(face_i);
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
            // Now perform the prediction, see how easy that is:
            int prediction = model->predict(face_resized);
            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            // Create the text we will annotate the box with:
            string box_text = format("Prediction = %d", prediction);
	    cout<<prediction<<endl;
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(face_i.tl().x - 10, 0);
            int pos_y = std::max(face_i.tl().y - 10, 0);
	    cout<<pos_x<<endl;
	    cout<<pos_y<<endl;
            // And now put it into the image:
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
        }
        // Show the result:
	vector<int> cp;
	cp.push_back(CV_IMWRITE_PNG_COMPRESSION);
	cp.push_back(3);
        imwrite("face_recognizer.png", original,cp);
	cout<<"After save"<<endl;
        // And display it:
        /*char key = (char) waitKey(20);
        if(key == 27)
            break;*/
	if(faceFound == 1) {
		break;
	}
    }
    cap.release();
    fclose(fp);
    printf("saved pic\n");
    printf("facefound is %d \n", faceFound);
    return faceFound;
}

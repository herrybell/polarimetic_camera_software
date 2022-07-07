#ifndef DEHAZE_H
#define DEHAZE_H


#include <QtGui>
#include <QDebug>
// OpenCV header files
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<cmath>
#include "omp.h"
using namespace cv;
using namespace std;
Mat dehaze( Mat I0, Mat I45,Mat I90,Mat I135);
Mat gammaTransform(Mat &srcImage, float kFactor);
void AWB(Mat& imageInput);
void ApplyCCMToImage(Mat& im_bgr);
#endif // DEHAZE_H

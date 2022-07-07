#ifndef POLARVISON_H
#define POLARVISON_H

#include <QtGui>
#include <QDebug>
// OpenCV header files
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
using namespace cv;
cv::Mat Dolp(Mat I0, Mat I45,Mat I90,Mat I135);
cv::Mat Aop(Mat I0, Mat I45,Mat I90,Mat I135);

#endif // POLARVISON_H

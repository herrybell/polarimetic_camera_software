#ifndef MATTOIMAGE_H
#define MATTOIMAGE_H

#include <QtGui>
#include <QDebug>
// OpenCV header files
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;

QImage MatToQImage(const Mat&);
cv::Mat QImageToMat(QImage image);
#endif // MATTOIMAGE_H

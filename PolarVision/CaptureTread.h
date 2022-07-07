#ifndef CAPTURETREAD_H
#define CAPTURETREAD_H

#include <QObject>
#include "ArenaApi.h"
#include <QThread>
#include <QtGui>
#include <QImage>
// OpenCV header files
#include "opencv/highgui.h"
#include "ImageBuffer.h"
using namespace std;
using namespace Arena;
using namespace cv;
class CaptureThread : public QThread
{

    Q_OBJECT
public:
     CaptureThread();
     ~CaptureThread();
     Arena::ISystem* pSystem;
     Arena::IDevice* pDevice;
     std::vector<Arena::DeviceInfo> deviceInfos;
     void GetImage(Arena::IDevice* pDevice, Mat& bayer_image0, Mat& bayer_image1, Mat& bayer_image2, Mat& bayer_image3);
     void WBStatsCollection(uint16_t* row0, uint16_t* row2);

     bool OpenCamera();
     bool StartCamera();
     void StopCamera();
     void CloseCamera();

     bool stopState;


     void stopCaptureThread();
signals:

public slots:
private:

    Mat grabbedFrame;
    uint64_t sumRed = 0;
    uint64_t sumGreen = 0;
    uint64_t sumBlue = 0;
    double redGain[4] = { 1.0, 1.0, 1.0, 1.0 };
    double greenGain[4] = { 1.0, 1.0, 1.0, 1.0 };
    double blueGain[4] = { 1.0, 1.0, 1.0, 1.0 };
    GenICam::gcstring pixelFormat = "PolarizeMono8";
    bool CameraRuning;
    bool CameraOpening;
    Arena::IImage* pImage;
    QImage QP0;
    QImage QP45;
    QImage QP90;
    QImage QP135;
    QImage  QP0_copy;
    QImage  QP45_copy;
    QImage  QP90_copy;
    QImage  QP135_copy;

protected:
    void run();
};

#endif // CAPTURETREAD_H

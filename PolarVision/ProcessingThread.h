#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

#include <QObject>
// Qt header files
#include <QThread>
#include <QtGui>
// OpenCV header files
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include "ImageBuffer.h"
#include<QTime>
using namespace cv;

class ProcessingThread : public QThread
{
    Q_OBJECT
public:
    explicit ProcessingThread();
    ~ProcessingThread();
    void stopProcessingThread();
    int getCurrentSizeOfBuffer();
    bool DeglintNow;
    bool DehazeNow;
    bool DolpNow;
    bool AopNow;
    int getprocessfps();
//        QTime t;
//        int captureTime;
         int FPS;
         int counts=0;

signals:
    void ShowOriSignal(Mat,Mat);
    void Show4PSignal(Mat,Mat,Mat,Mat);
    void ShowLoutSignal(Mat);
public slots:
private:
 //   ImageBuffer *imageBuffer;
  //  volatile bool stopped;
    int inputSourceWidth;
    int inputSourceHeight;
    int currentSizeOfBuffer;

    Mat currentFrame;
    Mat currentFrameGrayscale;

    Mat frame;
 //   QMutex stoppedMutex;
    QMutex updateMembersMutex;
    Size frameSize;
    Point framePoint;
    QString name;
    // Image processing flags

protected:
    void run();
};


#endif // PROCESSINGTHREAD_H

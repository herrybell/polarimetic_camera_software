#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "CaptureTread.h"
#include "ProcessingThread.h"
#include "ImageBuffer.h"

// Qt header files
#include <QtGui>
// OpenCV header files
#include <opencv/highgui.h>
using namespace cv;
class Controller : public QObject
{
    Q_OBJECT
public:
   Controller();
    ~Controller();
    ImageBuffer *imageBuffer;
    ProcessingThread *processingThread;
    CaptureThread *captureThread;
    bool connectToCamera(int,bool,int,int);
    void stopCaptureThread();
    void stopProcessingThread();
    void deleteCaptureThread();
    void deleteProcessingThread();
    void clearImageBuffer();
    void deleteImageBuffer();

signals:

public slots:
private:
    int imageBufferSize;
};

#endif // CONTROLLER_H

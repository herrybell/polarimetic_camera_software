#include "ProcessingThread.h"
#include "ImageBuffer.h"
#include "mattoimage.h"
#include "dehaze.h"
#include"deglint.h"
// Qt header files
#include <QDebug>
// OpenCV header files
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "globalh.h"
#include"polarvison.h"

ProcessingThread::ProcessingThread() : QThread()

{
 stopped1=false;
 DeglintNow=false;
 DehazeNow=false;
 DolpNow=false;
 AopNow=false;
 counts=0;

}
ProcessingThread::~ProcessingThread()
{
}
void ProcessingThread::run()
{
    while(1)
    {
        /////////////////////////////////
        // Stop thread if stopped=TRUE //测试是否停止的过程要加锁
        /////////////////////////////////

        stoppedMutex1.lock();
        if (stopped1)
        {
            stopped1=false;
            stoppedMutex1.unlock();
            break;
        }
        stoppedMutex1.unlock();
        /////////////////////////////////

        // Get frame from queue, store in currentFrame, set ROI
        currentFrame=Mat(imageBuffer->getFrame());//从缓冲区中取出一帧
        //currentFrame=imread("Penguins.jpg");


        updateMembersMutex.lock();
//        Rect por0(0, 0, 1024, 1224);
//        Rect por1(1035, 1224, 1024, 1224);
//        Rect por2(0, 1225, 1024, 1224);
//        Rect por3(1035, 1225, 1024, 1224);

        Mat P0 = currentFrame(Range(0,1024),Range(0,1224));//90
        Mat P1 = currentFrame(Range(1024,2048),Range(0,1224));//45
        Mat P2 = currentFrame(Range(0,1024),Range(1224,2448));//135
        Mat P3 = currentFrame(Range(1024,2048),Range(1224,2448));//0
       //cv::GaussianBlur(currentFrame, currentFrame, Size(201,201), 32);
        //以下的函数或者程序可以用来对每一帧图像进行处理
        if(DeglintNow)
       {
             frame=Delight(P3,P1,P0,P2);
        }
        if(DolpNow)
        {
         frame=Dolp(P3,P1,P0,P2);
        }
        if(DehazeNow){

         frame=dehaze(P3,P1,P0,P2);

     }
        if(AopNow){
         frame=Aop(P3,P1,P0,P2);
     }
        updateMembersMutex.unlock();
        // Convert Mat to QImage: Show BGR frame


        // Update statistics
        currentSizeOfBuffer=imageBuffer->getSizeOfImageBuffer();
        // Inform GUI thread of new frame (QImage)
        //发出信号，通知GUI线程有新处理好的一帧
        emit ShowOriSignal(P3,P0);
        emit Show4PSignal(P3,P1,P0,P2);
        emit ShowLoutSignal(frame);
        counts++;
    }
    qDebug() << "Stopping processing thread...";
}
void ProcessingThread::stopProcessingThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
} // stopProcessingThread()


int ProcessingThread::getCurrentSizeOfBuffer()
{
    return currentSizeOfBuffer;
} // getCurrentSizeOfBuffer()

int ProcessingThread::getprocessfps()
{
    return FPS;
}

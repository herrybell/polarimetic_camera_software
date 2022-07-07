#include "Controller.h"
#include "ImageBuffer.h"
#include "globalh.h"

Controller::Controller()
{

}
Controller::~Controller()
{
}
bool Controller::connectToCamera(int imageBufferSize, bool dropFrame, int capThreadPrio, int procThreadPrio)
{


    qDebug()<<"open camera sucessful"<<endl;
    captureThread = new CaptureThread();//新建捕获线程
    captureThread->OpenCamera();
    processingThread = new ProcessingThread();

    captureThread->start((QThread::Priority)capThreadPrio);//启动捕获进程

    processingThread->start((QThread::Priority)procThreadPrio);//启动处理进程

    return true;
}
void Controller::stopCaptureThread()
{
    qDebug() << "About to stop capture thread...";
    captureThread->stopCaptureThread();
    if(imageBuffer->getSizeOfImageBuffer()==imageBufferSize)
        cv::Mat temp=imageBuffer->getFrame();
    captureThread->wait();
    qDebug() << "Capture thread successfully stopped.";
} // stopCaptureThread()
void Controller::stopProcessingThread()
{
    qDebug() << "About to stop processing thread...";
    processingThread->stopProcessingThread();
    processingThread->wait();
    qDebug() << "Processing thread successfully stopped.";
} // stopProcessingThread()

void Controller::deleteCaptureThread()
{
    // Delete thread
    delete captureThread;
} // deleteCaptureThread()

void Controller::deleteProcessingThread()
{
    // Delete thread
    delete processingThread;
} // deleteProcessingThread()

void Controller::clearImageBuffer()
{
    imageBuffer->clearBuffer();
} // clearImageBuffer()

void Controller::deleteImageBuffer()
{
    // Delete image buffer
    delete imageBuffer;
} // deleteImageBuffer()

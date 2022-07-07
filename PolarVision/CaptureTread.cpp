#include "CaptureTread.h"
#include "ImageBuffer.h"
#include "opencv2/opencv.hpp"
#include "globalh.h"


// Qt header files
#include <QDebug>
int nPort=0;
CaptureThread::CaptureThread():QThread()
{
    CameraRuning = FALSE;
    CameraOpening = FALSE;
stopped=false;
}
CaptureThread::~CaptureThread()
{
    if(pDevice != NULL )
    {
        delete pDevice;
        pDevice = NULL;
        pSystem = NULL;
        CameraRuning = false;
        CameraOpening = false;
        stopState = true;
    }

    Arena::CloseSystem(pSystem);
}
bool CaptureThread::OpenCamera()
{
    if (!CameraOpening)
    {
        try
        {

            pSystem = Arena::OpenSystem();            
            pSystem->UpdateDevices(100);
            deviceInfos = pSystem->GetDevices();
             std::cout << "Using camera with serial number: " << deviceInfos[0].SerialNumber() << std::endl;


            if (deviceInfos.size() == 0)
            {
                std::cout << "\nNo camera connected\nPress enter to complete\n";
                std::getchar();
                return 0;
            }

            pDevice = pSystem->CreateDevice(deviceInfos[0]);


            CameraOpening = true;

           // GenICam::gcstring acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");

            Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", "Continuous");

            Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat", "PolarizeMono8");

            GenApi::CIntegerPtr pDeviceStreamChannelPacketSize = pDevice->GetNodeMap()->GetNode("DeviceStreamChannelPacketSize");
            pDeviceStreamChannelPacketSize->SetValue(9000);

            Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);
             Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize",true);
            /*
             bool acquisitionFrameRateEnableInitial =

             Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "AcquisitionFrameRateEnable");

             Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "AcquisitionFrameRateEnable", "true");

             //设置AcquisitionFrameRateEnable为true

             GenApi::CFloatPtr

             pAcquisitionFrameRate=pDevice->GetNodeMap()->GetNode("AcquisitionFrameRate");

             double acquisitionFrameRate = 22;

             pAcquisitionFrameRate ->SetValue(acquisitionFrameRate);*/
            Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "TriggerMode", "Off");

        }
        catch (GenICam::GenericException& ge)
        {
            cerr << "An exception occurred." << endl<< ge.GetDescription() << endl;
            cin.ignore(cin.rdbuf()->in_avail());
            CameraOpening = false;
            return false;
        }

    }


    return true;
}
bool CaptureThread::StartCamera()
{

    if (CameraOpening && !CameraRuning && pDevice)
    {

        pDevice->StartStream();

        CameraRuning = true;
        return true;
    }
    else
    {
        return false;
    }

}
void CaptureThread::StopCamera()
{
    if(pDevice)
    {
        stopState = true;
        pDevice->StopStream();
        CameraRuning = false;
    }
}
template<typename T>
T clip(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}

void CaptureThread::GetImage(IDevice* pDevice, Mat& bayer_image0, Mat& bayer_image1, Mat& bayer_image2, Mat& bayer_image3)
{


    int x0y0, x0y1, x1y0, x1y1;
    bool isBlueLine = false;
    bool isGreen = false;

    // get image
    pImage = pDevice->GetImage(500);

    unsigned char* inputBufferPtr = const_cast<uint8_t*>(pImage->GetData());
    uint32_t width = static_cast<uint32_t>(pImage->GetWidth());
    uint32_t height = static_cast<uint32_t>(pImage->GetHeight());

    bool changeTo16 = false;
    Arena::IImage* imageConverted;
    if (pixelFormat != "PolarizeMono16")
    {
        imageConverted = Arena::ImageFactory::Convert(pImage, Mono16);
        changeTo16 = true;
        inputBufferPtr = const_cast<uint8_t*>(imageConverted->GetData());
        width = static_cast<uint32_t>(imageConverted->GetWidth());
        height = static_cast<uint32_t>(imageConverted->GetHeight());
    }

    // process the image 4 pixels (0, 45, 90 & 135) at a time
    for (uint32_t ir = 0; ir < height; ir += 2)
    {
        uint16_t* row0 = reinterpret_cast<uint16_t*>(inputBufferPtr) + ir * width;
        uint16_t* row1 = reinterpret_cast<uint16_t*>(inputBufferPtr) + (ir + 1) * width;
        uint16_t* row2 = reinterpret_cast<uint16_t*>(inputBufferPtr) + (ir + 2) * width;

        unsigned char* binnedBufferPtr0 = bayer_image0.data + (ir / 2) * width / 2;
        unsigned char* binnedBufferPtr1 = bayer_image1.data + (ir / 2) * width / 2;
        unsigned char* binnedBufferPtr2 = bayer_image2.data + (ir / 2) * width / 2;
        unsigned char* binnedBufferPtr3 = bayer_image3.data + (ir / 2) * width / 2;


        for (uint32_t ic = 0; ic < width; ic += 2)
        {
            // calculate white balance and gain
            if (ir % 4 == 0 && ic % 4 == 0 && ir + 2 < height)
            {
                WBStatsCollection(row0 + ic, row2 + ic);
            }

            double* gain = isGreen ? greenGain : (isBlueLine ? blueGain : redGain);

            x0y0 = row0[ic];
            x0y1 = row0[ic + 1];
            x1y0 = row1[ic];
            x1y1 = row1[ic + 1];

            // binnedBufferPtr0[ic / 2] = (x0y0 + x0y1 + x1y0 + x1y1) / 4;
            binnedBufferPtr0[ic / 2] = static_cast<unsigned char>(clip<double>(x0y0 * gain[0] / 256 + 0.5, 0, 255));
            binnedBufferPtr1[ic / 2] = static_cast<unsigned char>(clip<double>(x0y1 * gain[1] / 256 + 0.5, 0, 255));
            binnedBufferPtr2[ic / 2] = static_cast<unsigned char>(clip<double>(x1y0 * gain[2] / 256 + 0.5, 0, 255));
            binnedBufferPtr3[ic / 2] = static_cast<unsigned char>(clip<double>(x1y1 * gain[3] / 256 + 0.5, 0, 255));


        }
        isGreen = !isGreen;
        isBlueLine = !isBlueLine;
    }

    // if the image is really dark or bright, there won't be enough pixels included in the sums of the red
    // green and blue pixel values and the gains will be noisy.  Only update the AWB gains if the sums are sufficiently
    // high, with the threshold set to 30000 pixels at a mid grey level (128 in 8-bit)
    const uint64_t minSum = 128 * 30000;
    if (sumBlue > minSum)
    {
        double globalRedGain = static_cast<double>(sumGreen) / static_cast<double>(sumRed);
        double globalBlueGain = static_cast<double>(sumGreen) / static_cast<double>(sumBlue);
        // update WB gains
        for (int i = 0; i < 4; i++)
        {
            if (true)
            {
                redGain[i] = globalRedGain;
                blueGain[i] = globalBlueGain;
            }
            else
            {
                redGain[i] = 1.0;
                blueGain[i] = 1.0;
            }
        }
    }

    sumRed = 0;
    sumGreen = 0;
    sumBlue = 0;

    if (pixelFormat != "PolarizeMono16")
    {
        Arena::ImageFactory::Destroy(imageConverted);
    }

    pDevice->RequeueBuffer(pImage);
}

void CaptureThread::WBStatsCollection(uint16_t* row0, uint16_t* row2)
{
    const uint16_t max_val = 250 << 8;

        bool red_good = (row0[0] < max_val);
        bool green_good = (row0[2] < max_val);
        bool blue_good = (row2[2] < max_val);

        // RRGG color polar,  RGGB bayer pattern with skiping 2 rows/cols
        if (red_good && green_good && blue_good)
        {
            sumRed += row0[0] >> 8;
            sumRed += row0[1] >> 8;

            sumGreen += row0[2] >> 8;
            sumGreen += row0[3] >> 8;

            sumBlue += row2[2] >> 8;
            sumBlue += row2[3] >> 8;
        }
}
void CaptureThread::CloseCamera()
{
    if(pDevice != NULL )
    {
        stopState = true;
        pSystem->DestroyDevice(pDevice);

        pDevice = NULL;
        CameraRuning = false;
        CameraOpening = false;

    }

    Arena::CloseSystem(pSystem);
}


void CaptureThread::run()
{
    if( !this->StartCamera() )
        return;

    stopState = false;

    GenICam::gcstring pixelFormatInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat");

        // Set pixel format
        //std::cout << "  " << "Set pixel format to '" << pixelFormat << "'\n";
        //Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "PixelFormat", pixelFormat);

        // Start stream
        std::cout << "  " << "Start stream\n";


        std::cout << "  " << "Get one image.\n";

        std::cout << "  " << "Streaming at " << Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "AcquisitionFrameRate") << " FPS" << std::endl;

        IImage* image = pDevice->GetImage(2000);


            uint64_t pixel_format;
            size_t height, width;
            pixel_format = image->GetPixelFormat();
            height = image->GetHeight();
            width = image->GetWidth();
            //size_t bits_per_pixel = image->GetBitsPerPixel();
            Mat bayer_image0(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_8UC1, Scalar::all(0));
             Mat bayer_image1(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_8UC1, Scalar::all(0));
             Mat bayer_image2(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_8UC1, Scalar::all(0));
             Mat bayer_image3(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_8UC1, Scalar::all(0));

             Mat color_image0(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_64FC3, Scalar::all(0));
             Mat color_image1(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_64FC3, Scalar::all(0));
             Mat color_image2(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_64FC3, Scalar::all(0));
             Mat color_image3(
              static_cast<int>(height / 2), static_cast<int>(width / 2), CV_64FC3, Scalar::all(0));

             Mat transportmat(
              static_cast<int>(height/2), static_cast<int>(width), CV_8UC3, Scalar::all(0));
             Mat h_color0(
              static_cast<int>(height / 2), static_cast<int>(width), CV_8UC3, Scalar::all(0));
             Mat h_color1(
              static_cast<int>(height / 2), static_cast<int>(width), CV_8UC3, Scalar::all(0));
            pDevice->RequeueBuffer(image);

while( !stopState )  {

        try {
            qDebug() << "当前线程对象的地址: " << QThread::currentThread();

            while (pDevice->IsConnected() )
            {
                GetImage(pDevice, bayer_image0, bayer_image1, bayer_image2, bayer_image3);

                // opencv bayer pattern specifies the second row / second+third column
                    cvtColor(bayer_image0, color_image0, CV_BayerBG2BGR);
                        //putText(color_image0, "Polarize 90", cvPoint(30, 30), FONT_HERSHEY_COMPLEX, 1.2, cvScalar(200, 200, 250), 1, CV_AA);
                    cvtColor(bayer_image1, color_image1, CV_BayerBG2BGR);
                        //putText(color_image1, "Polarize 45", cvPoint(30, 30), FONT_HERSHEY_COMPLEX, 1.2, cvScalar(200, 200, 250), 1, CV_AA);
                    cvtColor(bayer_image2, color_image2, CV_BayerBG2BGR);
                        //putText(color_image2, "Polarize 135", cvPoint(30, 30), FONT_HERSHEY_COMPLEX, 1.2, cvScalar(200, 200, 250), 1, CV_AA);
                    cvtColor(bayer_image3, color_image3, CV_BayerBG2BGR);
                        //putText(color_image3, "Polarize 0", cvPoint(30, 30), FONT_HERSHEY_COMPLEX, 1.2, cvScalar(200, 200, 250), 1, CV_AA);
                hconcat(color_image0, color_image1, h_color0);
                hconcat(color_image2, color_image3, h_color1);
                vconcat(h_color0, h_color1, transportmat);
                imageBuffer->addFrame( transportmat);
            }


        }

        catch (GenICam::GenericException& ge)
        {
            qDebug() << ge.GetDescription();
        }
    }

    qDebug() << "Exit" ;
}
void CaptureThread::stopCaptureThread()
{
    stoppedMutex.lock();
    stopped=true;
    stoppedMutex.unlock();
} // stopCaptureThread()


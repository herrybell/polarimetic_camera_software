#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mattoimage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{




    //setting for mainwindow--->
    ui->setupUi(this);
    //hide stop button
    ui->ButtonToDolpStop->hide();
     ui->ButtonToDehazeStop->hide();
      ui->ButtonToDeGlinStop->hide();
      ui->ButtonToODStop->hide();
      ui->ButtonToAopStop->hide();

     QMainWindow::setWindowTitle("pure");
    imageBufferSize=10;//定义缓冲区的大小
     this->setFixedSize(1200,750);
    //setting for mainwindow--->

    //ui setting
   ui->tabWidget->setStyleSheet(
                    "QTabWidget{"
                    "background-color:transparent;"
                    "}"
                    "QTabWidget::pane{"
                    "    border:2px;"
                    "border-style:outset;"
                    "background-color: rgb(31,25,25);"
                    "}"

                    "QTabBar::tab{"
                    "left:1px; "
                     "    background: rgb(31,25,25);"
                    "color: darkgray;"
                    "    min-width:28ex;"
                    "    min-height:10ex;"
                    "}"
                    "QTabBar::tab:hover{"
                    "    background:rgb(255, 255, 255, 100);"
                    "color: rgb(31,25,25);"
                    "}"
                    "QTabBar::tab:selected{"
                    "    border-color: black;"
                     "background:rgb(25, 109, 83);"
                    "    color:white;"
                    "}"
                    );
   ui->tabWidget_2->setStyleSheet(
                    "QTabWidget{"
                    "background-color:transparent;"
                    "}"
                    "QTabWidget::pane{"
                    "    border:2px;"
                    "border-style:outset;"
                    "background-color: rgb(31,25,25);"
                    "}"

                    "QTabBar::tab{"
                    "left:1px; "
                    "width: 240px;"
                     "    background: rgb(31,25,25);"
                    "color: darkgray;"
                    "    min-width:28ex;"
                    "    min-height:10ex;"
                    "}"
                    "QTabBar::tab:hover{"
                    "    background:rgb(255, 255, 255, 100);"
                    "color: rgb(31,25,25);"
                    "}"
                    "QTabBar::tab:selected{"
                    "    border-color: black;"
                     "background:rgb(25, 109, 83);"
                    "    color:white;"
                    "}"
                    );
 ui->toolBox->setStyleSheet(

             "QToolBox{"

             "background-color:transparent;"
             "}"
             "QToolBox::tab {"
                "background:rgb(31,25,25); "
                " border-radius: 5px;"
                 "color: darkgray;"
             "}"
             "QToolBox::pane{"
             "    border:2px;"
             "border-style:outset;"
             "background-color: rgb(31,25,25);"
             "}"
             "QToolBox::tab:hover{"
             "    background:rgb(255, 255, 255, 100);"
             "color: rgb(31,25,25);"
             "}"

            " QToolBox::tab:selected {"
             "background:rgb(25, 109, 83);"
                 "color:white;"
             "}"

             );


// QFrame, QLabel, QToolTip {
//     border: 2px solid green;
//     border-radius: 4px;
//     padding: 2px;
//     background-image: url(images/welcome.png);
// }

    //start GIF--->
    ui->LabelToStartGif->setScaledContents(true);
    m_movie = new QMovie("../sucai/backgroundimg6.gif");
    ui->LabelToStartGif->setMovie(m_movie);
    m_movie->start();
    connect(m_movie,SIGNAL(frameChanged(int)),this,SLOT(OnceMovie(int)));
    //start GIF--->
    \
     BackGroundImg.load("../sucai/blackflow2.png");
      setAutoFillBackground(true);
       QPalette pal(palette());
        pal.setBrush(QPalette::Window, QBrush(BackGroundImg.scaled(size(), Qt::IgnoreAspectRatio,
                          Qt::SmoothTransformation)));
       setPalette(pal);

 //ui setting
    controller = new Controller;//创建控制器
//menu list
//initial save setting
    noriframe=0;
    nproframe=0;
    nVoriframe=0;
    nVproframe=0;


}


MainWindow::~MainWindow()
{
    delete ui;
}



//***********DisPlaySlots  and Start/stop Porgram Func



void MainWindow::OnceMovie(int framenumber)
{
    if(m_movie->currentFrameNumber()==85)
    {m_movie->stop();
     ui->LabelToStartGif->hide();}
}
void MainWindow::startProgram(){

        if((isCameraConnected=controller->connectToCamera(imageBufferSize,false,7,7)))     //参数的意义见Imagebuffer的定义
        {
            qDebug()<<"mainwindow connect sucessfull"<<endl;
            //将处理线程的“产生新的一帧”信号和GUI线程的“更新帧”槽连接
//            connect(controller->processingThread,SIGNAL(newFrame(QImage)),this,SLOT(updateFrame(QImage)));
             //connect(controller->processingThread,SIGNAL(newFrame(Mat )), this, SLOT(updateFrame(Mat)));
            // Create connections (3) between GUI thread (emitter) and processing thread (receiver/listener)
            // Get input stream properties
}
        else
        {
            QMessageBox::warning(this,"ERROR:","Could not connect to camera.");
            // Delete dialogs
        }
}
void MainWindow::stopProgram()
{

    disconnect(controller->processingThread,SIGNAL(ShowLoutSignal(Mat)),0,0);
    disconnect(controller->processingThread,SIGNAL(Show4PSignal(Mat,Mat,Mat,Mat)),0,0);
    disconnect(controller->processingThread,SIGNAL(ShowOriSignal(Mat,Mat)),0,0);
    delete controller;
    this->close();



} // stopProgram()
void MainWindow::SlotDispOri(Mat color_image0,Mat color_image2){
    Mat original=color_image0+ color_image2;
    QPoriginal=MatToQImage(original);
    emit OriSave(QPoriginal);

            if (!QPoriginal.isNull())
            {

                pixmap90 = QPixmap::fromImage(QPoriginal);
                 ui->LabelToori->setScaledContents(true);
                ui->LabelToori->setPixmap(pixmap90);
            }



}
void MainWindow::SlotDisp4P(Mat color_image0,Mat color_image1,Mat color_image2,Mat color_image3){
    QP0=MatToQImage( color_image0);
    QP45=MatToQImage( color_image1);
    QP90=MatToQImage( color_image2);
    QP135=MatToQImage( color_image3);

        if (!QP0.isNull())
        {

            pixmap0 = QPixmap::fromImage(QP0);
            ui->LabelTo0->setScaledContents(true);
            ui->LabelTo0->setPixmap(pixmap0);

        }


        if (!QP45.isNull())
        {

            pixmap45 = QPixmap::fromImage(QP45);
            ui->LabelTo45->setScaledContents(true);
            ui->LabelTo45->setPixmap(pixmap45);

        }
        if (!QP90.isNull())
        {

            pixmap90 = QPixmap::fromImage(QP90);
            ui->LabelTo90->setScaledContents(true);
            ui->LabelTo90->setPixmap(pixmap90);

        }
        if (!QP135.isNull())
        {

            pixmap135 = QPixmap::fromImage(QP135);
            ui->LabelTo135->setScaledContents(true);
            ui->LabelTo135->setPixmap(pixmap135);

        }



}
void MainWindow::SlotDispDolp(Mat L_out){
    QL_out = MatToQImage(  L_out);



        if (!QL_out.isNull())
        {

            pixmaplout = QPixmap::fromImage(QL_out);
            ui->LabelToOut->setScaledContents(true);
            ui->LabelToOut->setPixmap(pixmaplout);

        }

    }
void MainWindow::SlotDispDehaze(Mat L_out){
    QL_out = MatToQImage(  L_out);
    emit DehazedSave(QL_out);


        if (!QL_out.isNull())
        {

            pixmaplout = QPixmap::fromImage(QL_out);
            ui->LabelToOut->setScaledContents(true);
            ui->LabelToOut->setPixmap(pixmaplout);

        }

    }
void MainWindow::SlotDispDeglint(Mat L_out){
    QL_out = MatToQImage(  L_out);



        if (!QL_out.isNull())
        {

            pixmaplout = QPixmap::fromImage(QL_out);
            ui->LabelToOut->setScaledContents(true);
            ui->LabelToOut->setPixmap(pixmaplout);

        }

    }
void MainWindow::SlotDispAop(Mat L_out){
    QL_out = MatToQImage(  L_out);



        if (!QL_out.isNull())
        {

            pixmaplout = QPixmap::fromImage(QL_out);
            ui->LabelToOut->setScaledContents(true);
            ui->LabelToOut->setPixmap(pixmaplout);

        }

    }
//***********DisPlaySlots  and Start/stop Porgram Func

//****************Save slots

void MainWindow::SaveOriImages(QImage Ori)
{

    std::string m_path = ImgDirPath.toStdString();
        std::string oripath = m_path +"ori_"+std::to_string(noriframe) + ".bmp";
        //QString SavePath = QString::fromStdString(m_path);
        QString oriSequenceSavePath = QString::fromStdString(oripath);       
        Ori.save(oriSequenceSavePath);
        noriframe++;
        disconnect(this,SIGNAL(OriSave(QImage)),this,SLOT(SaveOriImages(QImage)));
     
}
void MainWindow::SaveProImages(QImage Pro)
{

    std::string m_path = ImgDirPath.toStdString();
        std::string propath = m_path +"pro_"+std::to_string(nproframe) + ".bmp";
        //QString SavePath = QString::fromStdString(m_path);     
        QString proSequenceSavePath = QString::fromStdString(propath);
        Pro.save(proSequenceSavePath);
        nproframe++;
        disconnect(this,SIGNAL(DehazedSave(QImage)),this,SLOT(SaveProImages(QImage)));

}
void MainWindow::SaveOriVideo(Mat P3,Mat P0)
{


Ori=P3+P0;

                    if (Ori.data)//判断图片调入是否成功
                    {
                        cout << "ori writing..\n" << endl;
                        writer.write(Ori);

                    }

        //noriframe++;


}
void MainWindow::SaveProVideo(Mat Pro)
{


                if (Pro.data)//判断图片调入是否成功
                {
                    cout << "pro writing...\n" << endl;

                    writerdehaze.write(Pro);

                }
        //nproframe++;


}
//****************Button slots

void MainWindow::on_ButtonToOpenCamera_2_clicked()
{
    //controller->captureThread->OpenCamera();
}

void MainWindow::on_ButtonToStartStream_2_clicked()
{
     this->startProgram();
}


void MainWindow::on_ButtonToStopProgram_clicked()
{
    this->stopProgram();
}

void MainWindow::on_ButtonToCloseCamera_2_clicked()
{

}

void MainWindow::on_ButtonToOri_clicked()
{
    disconnect(controller->processingThread, SIGNAL(Show4PSignal(Mat,Mat,Mat,Mat)), this, SLOT(SlotDisp4P(Mat,Mat,Mat,Mat)));
    connect(controller->processingThread, SIGNAL(ShowOriSignal(Mat,Mat)), this, SLOT(SlotDispOri(Mat,Mat)));
        ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_ButtonTo4P_clicked()
{
    disconnect(controller->processingThread, SIGNAL(ShowOriSignal(Mat,Mat)), this, SLOT(SlotDispOri(Mat,Mat)));
        connect(controller->processingThread, SIGNAL(Show4PSignal(Mat,Mat,Mat,Mat)), this, SLOT(SlotDisp4P(Mat,Mat,Mat,Mat)));
        ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_ButtonToDolp_clicked()
{       ui->ButtonToDolpStop->show();
        disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDeglint(Mat)));
      disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDehaze(Mat)));
       controller->processingThread->DolpNow=true;
     connect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDolp(Mat)));

}

void MainWindow::on_ButtonToDolpStop_clicked()
{
    ui->ButtonToDolp->show();
    ui->ButtonToDolpStop->hide();
    controller->processingThread->DolpNow=false;
   disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDolp(Mat)));
   ui->LabelToOut->clear();

}

void MainWindow::on_ButtonToDehazeNow_clicked()
{
    ui->ButtonToDehazeStop->show();
    t.start();
    disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDeglint(Mat)));
     disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDolp(Mat)));

        controller->processingThread->DehazeNow=true;
        connect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDehaze(Mat)));
}

void MainWindow::on_ButtonToDehazeStop_clicked()
{
    ui->ButtonToDehazeNow->show();
    ui->ButtonToDehazeStop->hide();
    controller->processingThread->DehazeNow=false;
   disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDehaze(Mat)));
   ui->LabelToOut->clear();
}

void MainWindow::on_ButtonToDeGlint_clicked()
{

    ui->ButtonToDeGlinStop->show();
    disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDolp(Mat)));
    disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDehaze(Mat)));
    controller->processingThread->DeglintNow=true;
    connect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDeglint(Mat)));
}

void MainWindow::on_ButtonToDeGlinStop_clicked()
{
    ui->ButtonToDeGlint->show();
    ui->ButtonToDeGlinStop->hide();
    controller->processingThread->DeglintNow=false;
   disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDeglint(Mat)));
   ui->LabelToOut->clear();
}

void MainWindow::on_ButtonToAop_clicked()
{
    ui->ButtonToAopStop->show();
    disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDolp(Mat)));
    disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispDehaze(Mat)));
    controller->processingThread->AopNow=true;
    connect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispAop(Mat)));
}

void MainWindow::on_ButtonToAopStop_clicked()
{
    ui->ButtonToAop->show();
    ui->ButtonToAopStop->hide();
    controller->processingThread->AopNow=false;
   disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this, SLOT( SlotDispAop(Mat)));
   ui->LabelToOut->clear();
}


void MainWindow::on_ButtonToChooseImgPath_clicked()
{
     ImgDirPath = QFileDialog::getExistingDirectory(
                   this, "choose Images Directory",
                   "/");
       if (ImgDirPath.isEmpty())
       {
           return;
       }
       else
       {
           qDebug() << "srcDirPath=" << ImgDirPath;
           ImgDirPath += "/";
       }
}


void MainWindow::on_ButtonToCapture_clicked()
{
connect(this,SIGNAL(OriSave(QImage)),this,SLOT(SaveOriImages(QImage)));

connect(this,SIGNAL(DehazedSave(QImage)),this,SLOT(SaveProImages(QImage)));

}



void MainWindow::on_ButtonToChooseVidoPath_clicked()
{

    VioDirPath = QFileDialog::getExistingDirectory(
                  this, "choose Video Directory",
                  "/");
      if (VioDirPath.isEmpty())
      {
          return;
      }
      else
      {
          qDebug() << "srcDirPath=" << VioDirPath;
          VioDirPath += "/";
      }
}

void MainWindow::on_ButtonToStartVi_clicked()

{     mv_path = VioDirPath.toStdString();
      std::string mv_path2=mv_path;
    writer.open(mv_path+"ori.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10,
              cv::Size(1224, 1024), true);

  writerdehaze.open(mv_path2+"pro.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10,
              cv::Size(1224, 1024), true);
    connect(controller->processingThread, SIGNAL(ShowOriSignal(Mat,Mat)), this,SLOT(SaveOriVideo(Mat,Mat)));
    connect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this,SLOT(SaveProVideo(Mat)));
}

void MainWindow::on_ButtonToStopVi_clicked()
{
     writer.release();
     writerdehaze.release();
    disconnect(controller->processingThread, SIGNAL(ShowOriSignal(Mat,Mat)), this,SLOT(SaveOriVideo(Mat,Mat)));
   disconnect(controller->processingThread, SIGNAL(ShowLoutSignal(Mat)), this,SLOT(SaveProVideo(Mat)));

}

//****************Button slots

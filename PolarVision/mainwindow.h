#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include "Controller.h"
#include <iostream>
#include<QAction>
#include<QFileDialog>
#include <QTextCodec>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTime t;
    int captureTime;
     int FPS;
    QString ImgDirPath;
    QString VioDirPath;
    int noriframe;
    int nproframe;
    int nVoriframe;
    int nVproframe;
    std::string mv_path;
    std::string orivpath;
    std::string provpath;
    cv::VideoWriter writer;
     cv::VideoWriter writerdehaze;
private:
    Ui::MainWindow *ui;
    Controller *controller;
    int sourceWidth;
    int sourceHeight;
    int imageBufferSize;
    bool isCameraConnected;
    QMovie *m_movie;
    QImage QL_out;
    QPixmap pixmap0;
    QPixmap pixmap45;
    QPixmap pixmap90;
    QPixmap pixmap135;
    QPixmap pixmaplout;
    QImage QP0;
    QImage QP45;
    QImage QP90;
    QImage QP135;
    QImage BackGroundImg;
    QImage QPoriginal;
    Mat Ori;


signals:
    void OriSave(QImage);
    void DehazedSave(QImage);
public slots:
    void startProgram();
    void stopProgram();

private slots:
    void OnceMovie(int framenumber);
    //Menu slots

    //display label slots
    void SlotDispOri(Mat color_image0,Mat color_image2);
    void SlotDisp4P(Mat color_image0,Mat color_image1,Mat color_image2,Mat color_image3);
    void SlotDispDolp(Mat L_out);
    void SlotDispDehaze(Mat L_out);
    void SlotDispDeglint(Mat L_out);
    void SlotDispAop(Mat L_out);
    //Save Slots
    void SaveProImages(QImage Pro);
    void SaveOriImages(QImage Ori);
    void SaveOriVideo(Mat P3,Mat P0);
    void SaveProVideo(Mat Pro);
    //button slots
    void on_ButtonToOpenCamera_2_clicked();
    void on_ButtonToStartStream_2_clicked();
    void on_ButtonToStopProgram_clicked();
    void on_ButtonToCloseCamera_2_clicked();
    void on_ButtonToOri_clicked();
    void on_ButtonTo4P_clicked();
    void on_ButtonToDolp_clicked();
    void on_ButtonToDolpStop_clicked();
    void on_ButtonToDehazeNow_clicked();
    void on_ButtonToDehazeStop_clicked();
    void on_ButtonToDeGlint_clicked();
    void on_ButtonToDeGlinStop_clicked();
    void on_ButtonToAop_clicked();
    void on_ButtonToAopStop_clicked();
    void on_ButtonToChooseImgPath_clicked();
    void on_ButtonToCapture_clicked();
    void on_ButtonToChooseVidoPath_clicked();
    void on_ButtonToStartVi_clicked();
    void on_ButtonToStopVi_clicked();
};

#endif // MAINWINDOW_H

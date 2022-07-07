#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Mat>("Mat");
    qRegisterMetaType<Mat>("Mat&");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

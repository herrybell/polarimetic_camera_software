#-------------------------------------------------
#
# Project created by QtCreator 2021-11-09T09:18:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pure
TEMPLATE = app
CONFIG += c++11
#openmp
QMAKE_CXXFLAGS += -openmp
QMAKE_LFLAGS += -openmp
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    globalh.cpp \
    mattoimage.cpp \
    Controller.cpp \
    CaptureTread.cpp \
    ProcessingThread.cpp \
    ImageBuffer.cpp \
    polarvison.cpp

HEADERS += \
        mainwindow.h \
    globalh.h \
    mattoimage.h \
    Controller.h \
    CaptureTread.h \
    ProcessingThread.h \
    ImageBuffer.h \
    dehaze.h \
    deglint.h \
    polarvison.h

FORMS += \
        mainwindow.ui
INCLUDEPATH += $$PWD/../include/Arena $$PWD/../GenICam/library/CPP/include
DEPENDPATH += $$PWD/../include/Arena $$PWD/../GenICam/library/CPP/include
win32: LIBS += -L$$PWD/../lib64/Arena/ -lArena_v140 -lGenTL_LUCID_v140 \
                -L$$PWD/../GenICam/library/CPP/lib/Win64_x64/ -lGCBase_MD_VC140_v3_1_LUCID \
               -lGenApi_MD_VC140_v3_1_LUCID \
                -lLog_MD_VC140_v3_1_LUCID \
                -lMathParser_MD_VC140_v3_1_LUCID \
                -lNodeMapData_MD_VC140_v3_1_LUCID \
                -lResUsageStat_MD_VC140_v3_1_LUCID \
                -lXmlParser_MD_VC140_v3_1_LUCID
INCLUDEPATH +=D:/gsb/qt_pro/OpenCV/include D:/gsb/qt_pro/OpenCV/include/opencv D:/gsb/qt_pro/OpenCV/include/opencv2
win32: LIBS +=D:/gsb/qt_pro/OpenCV/x64/vc14/lib/opencv_world343d.lib
win32: LIBS +=D:/gsb/qt_pro/OpenCV/x64/vc14/lib/opencv_world343.lib

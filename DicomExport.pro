#-------------------------------------------------
#
# Project created by QtCreator 2010-08-06T17:25:22
#
#-------------------------------------------------

QT       += core gui

TARGET = DicomExport
TEMPLATE = app



SOURCES += main.cpp\
        dicomwindow.cpp \
    dicomexporter.cpp \
    aboutdialog.cpp

HEADERS  += dicomwindow.h \
    dicomexporter.h \
    aboutdialog.h

FORMS    += dicomwindow.ui \
    aboutdialog.ui
unix {
    LIBS += -lmdc
}
win32 {
    LIBS += "C:/Program Files/XMedCon/bin/libmdc-2.dll" -lmdc
    INCLUDEPATH += "C:/Program Files/XMedCon/include"
}

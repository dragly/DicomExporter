#include <QtGui/QApplication>
#include "dicomwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DicomWindow w;
    w.show();

    return a.exec();
}

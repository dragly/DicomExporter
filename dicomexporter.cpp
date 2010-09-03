#include "dicomexporter.h"
#include <medcon.h>
#include <QImage>
#include <QDebug>
#include <QStringList>
#include <QThread>

DicomExporter::DicomExporter(QObject *parent) :
        QObject(parent)
{
    this->parent = parent;
}
void DicomExporter::exportFiles(FILEINFO *file, QString outFile, int eachFrame, bool resetFrameCount) {
    qDebug() << this->thread()->currentThreadId();
    int exportedFrames = 0;
    qDebug() << "Exporting with each" << eachFrame << "frame out of n =" << file->number << "frames";
    for(int i = 0; i < (int)file->number; i = i + eachFrame) {
        if((i + 1) % eachFrame || eachFrame == 1) {
            qDebug() << "Exporting frame number" << i;
            // append number
            QString fileName = outFile;
            QStringList fileNameSplit = fileName.split(".");
            int numberOfSplits = fileNameSplit.count();
            QString outFileName;
            QString numberString;
            for(int s = 0; s < numberOfSplits; s++) {
                outFileName.append(fileNameSplit[s]);
                if(s == numberOfSplits - 2) {
                    if(resetFrameCount) {
                        numberString = QString::number(exportedFrames);
                    } else {
                        numberString = QString::number(i);
                    }
                    while(numberString.length() < 4) {
                        numberString.prepend("0"); // add leading zeros
                    }
                    outFileName.append("-" + numberString + ".");
                } else if(s != numberOfSplits - 1) {
                    outFileName.append(".");
                }
            }
            emit setProgressText("Exporting frame number " + QString::number(i) + " as " + numberString);
            QImage img(file->image[i].buf, file->image[i].width, file->image[i].height, QImage::Format_RGB888);
            qDebug() << outFileName;
            img.save(outFileName);
            exportedFrames++;
            float progressValue = 100 * (float) exportedFrames / (float) (file->number / eachFrame);
            emit setProgress((int)progressValue);
        }
    }
}

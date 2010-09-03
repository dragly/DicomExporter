#ifndef DICOMEXPORTER_H
#define DICOMEXPORTER_H

#include <QObject>
#include <medcon.h>

class DicomExporter : public QObject
{
    Q_OBJECT
public:
    explicit DicomExporter(QObject *parent = 0);

private:
    QObject *parent;

signals:
    void setProgress(int value);
    void setProgressText(QString text);

public slots:
    void exportFiles(FILEINFO* inFile, QString outFile, int eachFrame, bool resetFrameCount);

};

#endif // DICOMEXPORTER_H

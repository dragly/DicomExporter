#include "dicomwindow.h"
#include "ui_dicomwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <medcon.h>

DicomWindow::DicomWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::DicomWindow)
{
    ui->setupUi(this);
    file = new FILEINFO;
}

DicomWindow::~DicomWindow()
{
    delete ui;
}

void DicomWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DicomWindow::on_btnOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DICOM file"));
    if (MdcOpenFile(file,fileName.toLatin1()) != MDC_OK) {
        QMessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText("Error in opening file");
        msg.exec();
        return;
    }
    if (MdcReadFile(file,0,NULL) != MDC_OK) {
        QMessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText("Error in reading file");
        msg.exec();
        return;
    }
}

void DicomWindow::on_btnExport_clicked()
{
    qDebug() << "Going to export";
    int eachFrame = ui->spinBox->value();
    int exportedFrames = 0;
    for(int i = 0; i < (int)file->number; i = i + ui->spinBox->value()) {
        if((i + 1) % eachFrame) {
            qDebug() << "Exporting frame number" << i;
            exportedFrames++;
            QImage img(file->image[i].buf, file->image[i].width, file->image[i].height, QImage::Format_RGB888);
            // append number
            QString fileName = ui->txtFileName->text();
            QStringList fileNameSplit = fileName.split(".");
            int numberOfSplits = fileNameSplit.count();
            QString outFileName;
            for(int s = 0; s < numberOfSplits; s++) {
                outFileName.append(fileNameSplit[s]);
                if(s == numberOfSplits - 2) {
                    QString numberString;
                    if(ui->chkResetFrameCount->isChecked()) {
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
            qDebug() << outFileName;
            qDebug() << img.save(outFileName);
        }
    }
}

void DicomWindow::on_btnBrowseExport_clicked()
{
    ui->txtFileName->setText(QFileDialog::getSaveFileName(this, tr("Save file"), QString(), tr("Image files (*.png *.jpg)")));
}

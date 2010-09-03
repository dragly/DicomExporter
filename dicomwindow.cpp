#include "dicomwindow.h"
#include "ui_dicomwindow.h"
#include "dicomexporter.h"
#include "aboutdialog.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

#include <medcon.h>

DicomWindow *dicomWindow_callback;

void updateProgressBar_callback(int type, float value, char *label)
{
    dicomWindow_callback->updateProgressBar(type, value, label);
}

DicomWindow::DicomWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::DicomWindow)
{
    ui->setupUi(this);
    file = new FILEINFO;
    dicomWindow_callback = this;
    progressValue = 0;
    MDC_PROGRESS = 1;
    MdcProgress = updateProgressBar_callback;
    connect(this, SIGNAL(setProgress(int)), ui->progressBar, SLOT(setValue(int)));
    exportThread = new QThread(this);
    exporter = new DicomExporter(0);
    exportThread->start();
    exporter->moveToThread(exportThread);
    connect(exporter, SIGNAL(setProgress(int)),ui->progressBar, SLOT(setValue(int)));
    connect(exporter, SIGNAL(setProgressText(QString)), this, SLOT(setProgressText(QString)));
    connect(this, SIGNAL(exportFiles(FILEINFO*, QString, int, bool)), exporter, SLOT(exportFiles(FILEINFO*, QString, int, bool)));
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

void DicomWindow::setProgressText(QString text) {
    ui->progressLabel->setText(text);
}

void DicomWindow::on_btnOpenFile_clicked()
{
    ui->progressLabel->setText("Opening file");
    ui->progressBar->setValue(0);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DICOM file"));
    ui->progressLabel->setText("Reading file");
    if(!fileName.isNull()) {
        if (MdcOpenFile(file,fileName.toLatin1()) != MDC_OK) {
            ui->progressLabel->setText("Error opening file");
            ui->progressBar->setValue(0);
            return;
        }
        if (MdcReadFile(file,0,NULL) != MDC_OK) {
            ui->progressLabel->setText("Error reading file");
            ui->progressBar->setValue(0);
            return;
        }
        ui->progressLabel->setText("File read successfully");
    } else {
        ui->progressLabel->setText("Ready");
        ui->progressBar->setValue(0);
    }
}

void DicomWindow::on_btnExport_clicked()
{
    qDebug() << "Going to export" << this->thread()->currentThreadId();
    int eachFrame = ui->spinBox->value();
    emit exportFiles(file, ui->txtFileName->text(), eachFrame, ui->chkResetFrameCount->isChecked());
    ui->progressLabel->setText("Export finished");
    emit setProgress(100);
}

void DicomWindow::on_btnBrowseExport_clicked()
{
    ui->txtFileName->setText(QFileDialog::getSaveFileName(this, tr("Save file"), QString(), tr("Image files (*.png *.jpg)")));
}

void DicomWindow::updateProgressBar(int type, float value, char *label)
{
    switch (type) {
    case MDC_PROGRESS_BEGIN:
        progressValue = 0;
        ui->progressLabel->setText(QString::fromLatin1(label));
        break;
    case MDC_PROGRESS_SET  :
        progressValue = value;
        break;
    case MDC_PROGRESS_INCR :
        progressValue += value;
        break;
    case MDC_PROGRESS_END  :
        progressValue = 100;
        break;
    }
    emit setProgress((int)(progressValue * 100));

}

void DicomWindow::on_actionAbout_triggered()
{
    AboutDialog *about = new AboutDialog(this);
    about->exec();
}

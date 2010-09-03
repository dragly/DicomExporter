#ifndef DICOMWINDOW_H
#define DICOMWINDOW_H

#include <QMainWindow>
#include <medcon.h>

namespace Ui {
    class DicomWindow;
}

class DicomExporter;

class DicomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DicomWindow(QWidget *parent = 0);
    ~DicomWindow();

    void updateProgressBar(int type, float value, char *label);
public slots:
    void setProgressText(QString text);

signals:
    void setProgress(int value);
    void exportFiles(FILEINFO* infile, QString outfile, int eachFrame, bool resetFrameNumbers);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DicomWindow *ui;

    FILEINFO *file;
    float progressValue;
    DicomExporter *exporter;
    QThread *exportThread;

private slots:
    void on_actionAbout_triggered();
    void on_btnBrowseExport_clicked();
    void on_btnExport_clicked();
    void on_btnOpenFile_clicked();
};

#endif // DICOMWINDOW_H

#ifndef DICOMWINDOW_H
#define DICOMWINDOW_H

#include <QMainWindow>
#include <medcon.h>

namespace Ui {
    class DicomWindow;
}

class DicomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DicomWindow(QWidget *parent = 0);
    ~DicomWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DicomWindow *ui;

    FILEINFO *file;

private slots:
    void on_btnBrowseExport_clicked();
    void on_btnExport_clicked();
    void on_btnOpenFile_clicked();
};

#endif // DICOMWINDOW_H

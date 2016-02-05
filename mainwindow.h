#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

#include "converter.h"
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_addFile_pressed();
    void plotMousePress(QCPAbstractPlottable*, QMouseEvent *event);

private:
    QDir _filePath;
    Ui::MainWindow *ui;
    QFileSystemModel *filemodel;
    QVector<SensorData> *sensorDataForView;

};

#endif // MAINWINDOW_H

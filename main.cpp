#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumWidth(1024);
    w.setMinimumHeight(640);
    w.show();

    return a.exec();
}

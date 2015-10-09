#include <QtWidgets>
#include <QtWebKitWidgets/QWebView>
#include <QWebEngineView>
#include <QDebug>
#include <QUrl>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addfile.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);

    //ui->cesiumView->load(QUrl("http://cesiumjs.org/Cesium/Build/Apps/CesiumViewer/index.html"));

    //sui->cesiumView->load(QUrl("file:///Users/erik-falk/Qt_Projects/SAM_Sensorauswertung/cesium.html"));

    ui->cesiumView->load(QUrl("file:///Users/erik-falk/Qt_Projects/SAM_Sensorauswertung/Cesium/Apps/CesiumViewer/index.html"));

    filemodel = new QFileSystemModel(this);

    filemodel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    filemodel->setNameFilters(QStringList() << "*.czml");
    filemodel->setNameFilterDisables(true);

    ui->treeView->setModel(filemodel);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setRootIndex("../SAM_Sensorauswertung/czml");
    //ui->treeView->setRootIndex(filemodel->setRootPath(QDir::rootPath()));
    ui->treeView->setAutoScroll(true);

}

MainWindow::~MainWindow()
{
    delete filemodel;
    delete ui;
}



void MainWindow::on_btn_addFile_pressed()
{
 AddFile addfile;
 addfile.exec();
}

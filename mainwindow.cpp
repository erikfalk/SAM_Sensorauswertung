#include <QtWidgets>
#include <QWebView>
#include <QDebug>
#include <QUrl>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addfile.h"
#include "cesiumwebview.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /*
    QWebEngineSettings *settings = ui->cesiumView->settings();

    settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    settings->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
    settings->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);


    ui->cesiumView->setMouseTracking(true);
    */

    ui->cesiumView->setAcceptDrops(true);
    ui->cesiumView->load(QUrl("http://cesiumjs.org/Cesium/Build/Apps/CesiumViewer/index.html"));
    //ui->cesiumView->dropEvent();



    //ui->cesiumView->load(QUrl("http://cesiumjs.org/Cesium/Build/Apps/CesiumViewer/index.html"));

    //ui->cesiumView->load(QUrl("file:///Users/erik-falk/Qt_Projects/SAM_Sensorauswertung/cesium.html"));

    //ui->cesiumView->load(QUrl("file:///Users/erik-falk/Qt_Projects/SAM_Sensorauswertung/Cesium/Apps/CesiumViewer/index.html"));

    filemodel = new QFileSystemModel(this);

    filemodel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    filemodel->setNameFilters(QStringList() << "*.czml");
    filemodel->setNameFilterDisables(true);


    ui->treeView->setModel(filemodel);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setRootIndex(filemodel->setRootPath(QDir::rootPath()));
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



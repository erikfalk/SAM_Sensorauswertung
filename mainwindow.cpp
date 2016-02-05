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

    Converter test;
    //test.readCzml("/Users/erik-falk/Desktop/SAM_Testdaten.czml");

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

    //get data directory
    QString appdir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    //check if czml directory exist
    QDir czml;
    if (!czml.exists(appdir))
        czml.mkdir(appdir);
    //add directory name to pathstring
    appdir += "/czml";

    _filePath.setPath(appdir);
    // create dir in case it doesn't exist
    if(!_filePath.exists())
        _filePath.mkdir(appdir);



    filemodel = new QFileSystemModel(this);

    filemodel->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    filemodel->setNameFilters(QStringList() << "*.czml");
    filemodel->setNameFilterDisables(true);


    ui->treeView->setModel(filemodel);
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    qDebug() << appdir;
    ui->treeView->setRootIndex(filemodel->setRootPath(appdir));
    ui->treeView->setAutoScroll(true);

    connect(ui->chartWidget, &QCustomPlot::plottableClick, this, &MainWindow::plotMousePress);

}

MainWindow::~MainWindow()
{
    delete filemodel;
    delete ui;
    delete sensorDataForView;
}


void MainWindow::on_btn_addFile_pressed()
{
 AddFile addfile(_filePath);
 addfile.exec();
}

void MainWindow::plotMousePress(QCPAbstractPlottable* plottable, QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        if(plottable)
        {
            double x = ui->chartWidget->xAxis->pixelToCoord(event->pos().x());
            double y = ui->chartWidget->yAxis->pixelToCoord(event->pos().y());

            QCPBars *bar = qobject_cast<QCPBars*>(plottable);

            double key = 0;
            double value = 0;
            QString xValue;

            bool ok = false;
            double m = std::numeric_limits<double>::max();

            foreach(QCPBarData data, bar->data()->values())
                {
                    double d = qAbs(x - data.key);

                    if(d < m)
                    {

                        key = data.key;
                        value = data.value;

                        ok = true;
                        m = d;
                    }
                }

            if(ok)
            {
               for(QCPBars *below = bar->barBelow();
                  ((below != NULL) && below->data()->contains(key));
                    below = below->barBelow())
                    {
                        value += below->data()->value(key).value;
                    }

                    QToolTip::hideText();
                    QToolTip::showText(event->globalPos(),
                    tr("<table>"
                         "<tr>"
                           "<th colspan=\"2\">%L1</th>"
                         "</tr>"
                         "<tr>"
                           "<td>Key:</td>" "<td>%L2</td>"
                         "</tr>"
                         "<tr>"
                           "<td>Val:</td>" "<td>%L3</td>"
                         "</tr>"
                       "</table>").
                       arg(bar->name().isEmpty() ? "..." : bar->name()).
                       arg(key).
                       arg(ui->chartWidget->xAxis->tickVectorLabels().at(key)),
                       ui->chartWidget, ui->chartWidget->rect());
            }
         }
    }
}

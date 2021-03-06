#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/pics/icon/SAM.png"));
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
    ui->treeView->setRootIndex(filemodel->setRootPath(appdir));
    ui->treeView->setAutoScroll(true);

    connect(ui->chartWidget, &QCustomPlot::plottableDoubleClick, this, &MainWindow::plotDblMousePress);
    connect(ui->chartWidget, &QCustomPlot::plottableClick, this, &MainWindow::plotMousePress);
    connect(ui->cesiumView, &CesiumWebView::sendSensorData, this, &MainWindow::onSensorDataRecieved);
}

MainWindow::~MainWindow()
{
    delete filemodel;
    delete ui;
}


void MainWindow::on_btn_addFile_pressed()
{
 AddFile addfile(_filePath);
 addfile.exec();
}

void MainWindow::plotMousePress(QCPAbstractPlottable *plottable, QMouseEvent *event) {

    if(event->button() == Qt::RightButton) {
       if(plottable) {

           double x = ui->chartWidget->xAxis->pixelToCoord(event->pos().x());
           QCPBars *bar = qobject_cast<QCPBars*>(plottable);

           int key = 0;
           double value = 0;

           bool ok = false;
           double m = std::numeric_limits<double>::max();

           foreach(QCPBarData data, bar->data()->values()) {

              double d = qAbs(x - data.key);

              if(d < m) {

                key = data.key;
                value = data.value;

                ok = true;
                m = d;
              }
            }

            if(ok) {

              for(QCPBars *below = bar->barBelow();
                 ((below != NULL) && below->data()->contains(key));
                 below = below->barBelow()) {

                    value += below->data()->value(key).value;
              }

              QToolTip::hideText();
              QToolTip::showText(event->globalPos(),
               tr("<table>"
                    "<tr>"
                     "<th colspan=\"2\">Id: %L1</th>"
                    "</tr>"
                    "<tr>"
                     "<td>Sensorvalue:</td>" "<td>%L2</td>"
                    "</tr>"
                   "</table>").

                 arg(ui->chartWidget->xAxis->tickVectorLabels().at(key)).
                 arg(value),
                     ui->chartWidget, ui->chartWidget->rect());
            }
      }
    }
}


void MainWindow::plotDblMousePress(QCPAbstractPlottable* plottable, QMouseEvent *event) {

    if(event->button() == Qt::LeftButton) {
       if(plottable) {

           double x = ui->chartWidget->xAxis->pixelToCoord(event->pos().x());
           QCPBars *bar = qobject_cast<QCPBars*>(plottable);

           int key = 0;
           double value = 0;

           bool ok = false;
           double m = std::numeric_limits<double>::max();

           foreach(QCPBarData data, bar->data()->values()) {

              double d = qAbs(x - data.key);

              if(d < m) {

                key = data.key;
                value = data.value;

                ok = true;
                m = d;
              }
            }

            if(ok) {

              for(QCPBars *below = bar->barBelow();
                 ((below != NULL) && below->data()->contains(key));
                 below = below->barBelow()) {

                    value += below->data()->value(key).value;
              }

              double id = ui->chartWidget->xAxis->tickVectorLabels().at(key).toDouble();
              for(int i = 0; i < _loadedSensorData.count(); i++) {
                   if(id == _loadedSensorData.at(i).getId()) {
                       showLocationOnMap(_loadedSensorData.at(i).getPosition());
                       break;
                   }
             }
          }
    }
  }
}

void MainWindow::showLocationOnMap(QGeoCoordinate location) {

    ui->cesiumView->setFocus();
    QWebElement webelement;
    QWebFrame* frame = ui->cesiumView->page()->currentFrame();

    webelement = frame->findFirstElement("input[type=search]");
    webelement.setFocus();
    webelement.setAttribute("value", QString::number(location.longitude()) + ","
                            + QString::number(location.latitude()));

    QKeyEvent *press[10];
    for(int i = 0; i < 10; i++){
        press[i] = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        qApp->postEvent(ui->cesiumView, press[i]);
    }
}


void MainWindow::onSensorDataRecieved(QVector<SensorData> &data) {
    if(!_loadedSensorData.isEmpty())
       _loadedSensorData.clear();

    _loadedSensorData = data;
}


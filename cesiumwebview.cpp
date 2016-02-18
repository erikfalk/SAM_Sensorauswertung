#include "cesiumwebview.h"

CesiumWebView::CesiumWebView(QWidget *parent) : QWebView(parent) {

}

void CesiumWebView::dropEvent(QDropEvent *event){

  //get filepath of droped file
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();

    // extract the local path of draged file
    for (int i = 0; i < urlList.size(); i++) {
        pathList.append(urlList.at(i).toLocalFile());
    }

        if(pathList.at(0).endsWith(".czml")) {

            QVector<SensorData> sensordata;
            CzmlReader czmlReader;

            QWebView::dropEvent(event);
            czmlReader.read(pathList.at(0), sensordata);
            drawDataToChart(sensordata);
            emit sendSensorData(sensordata);

        }
        else {
            QFileInfo fi(pathList.at(0));
            QMessageBox::warning(this, "Warning", fi.fileName() + "\nis not a czml file!");
        }
  }
}

void CesiumWebView::drawDataToChart(QVector<SensorData> &data)
{

    QVector<QString> valuePosition;
    QVector<double> sensorValue, ticks;

    QCustomPlot *plot = this->parentWidget()->findChild<QCustomPlot *>("chartWidget");

    QCPBars *sensorValueBars = new QCPBars(plot->xAxis, plot->yAxis);
    sensorValueBars->setSelectable(true);
    plot->clearPlottables();
    plot->addPlottable(sensorValueBars);

    //prepare x-axis
    for(int i = 0; i < data.count(); i++){
        if(data[i].getSensorValue() != std::numeric_limits<double>::min()){
            valuePosition << QString::number(data.at(i).getId());
        }
        ticks << i;
    }

    plot->xAxis->setAutoTicks(false);
    plot->xAxis->setAutoTickLabels(false);
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickVector(ticks);
    plot->xAxis->setTickVectorLabels(valuePosition);
    plot->xAxis->setTickStep(data.count());
    plot->xAxis->setTickLabelRotation(60);
    plot->xAxis->setSubTickCount(0);
    plot->xAxis->setTickLength(0, 4);
    plot->xAxis->grid()->setVisible(false);
    plot->xAxis->setRange(0, data.count());

    //prepare y-axis
    for(int i = 0; i < data.count(); i++){
        if(data[i].getSensorValue() != std::numeric_limits<double>::min())
            sensorValue << data[i].getSensorValue();
    }

    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(255, 131, 0));
    sensorValueBars->setPen(pen);
    sensorValueBars->setBrush(QColor(255, 131, 0, 50));

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    plot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    plot->yAxis->grid()->setSubGridPen(gridPen);

    CzmlConverter converter;
    converter.findExtrema(data);
    plot->yAxis->setRange(converter.getMinSensorValue(), converter.getMaxSensorValue());
    //plot data
    sensorValueBars->setData(ticks, sensorValue);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->replot();
}







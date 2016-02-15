#include "cesiumwebview.h"

CesiumWebView::CesiumWebView(QWidget *parent) : QWebView(parent) {

}

void CesiumWebView::dropEvent(QDropEvent *event){

  QWebView::dropEvent(event);

  //get filepath of draged file
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();
    CzmlReader czmlReader;

    // extract the local paths of draged file
    for (int i = 0; i < urlList.size(); i++) {
        pathList.append(urlList.at(i).toLocalFile());
    }
        QVector<SensorData> sensordata;

        czmlReader.read(pathList.at(0), sensordata);
        emit sendSensorData(sensordata);
        drawDataToChart(sensordata);
  }
}

void CesiumWebView::drawDataToChart(QVector<SensorData> &data)
{

    QCustomPlot *plot = this->parentWidget()->findChild<QCustomPlot *>("chartWidget");
    QVector<QString> valuePosition;
    QVector<double> sensorValue, ticks;

    QCPBars *sensorValueBars = new QCPBars(plot->xAxis, plot->yAxis);
    sensorValueBars->setSelectable(true);
    plot->addPlottable(sensorValueBars);

    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(255, 131, 0));
    sensorValueBars->setPen(pen);
    sensorValueBars->setBrush(QColor(255, 131, 0, 50));

    //prepare x-axis
    for(int i = 0; i < data.count(); i++){
       ticks << i;
       valuePosition << QString::number(data.at(i).getId());
    }
    plot->xAxis->setAutoTicks(false);
    plot->xAxis->setAutoTickLabels(false);
    plot->xAxis->setTickVector(ticks);
    plot->xAxis->setTickVectorLabels(valuePosition);
    plot->xAxis->setTickLabelRotation(60);
    plot->xAxis->setSubTickCount(0);
    plot->xAxis->setTickLength(0, 4);
    plot->xAxis->grid()->setVisible(false);
    plot->xAxis->setRange(0, data.count());

    //prepare y-axis
    for(int i = 0; i < data.count(); i++){
        sensorValue << data[i].getSensorValue();
    }
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    plot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    plot->yAxis->grid()->setSubGridPen(gridPen);

    CzmlConverter converter;
    converter.findExtrema(data);
    plot->yAxis->setRange(converter.getMinSensorValue(), converter.getMaxSensorValue());
    sensorValueBars->setData(ticks, sensorValue);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->replot();
}







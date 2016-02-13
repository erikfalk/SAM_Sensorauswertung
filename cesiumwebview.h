#ifndef CESIUMWEBVIEW_H
#define CESIUMWEBVIEW_H

#include <QWidget>
#include <QWebView>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>

#include "czmlreader.h"
#include "czmlconverter.h"
#include "qcustomplot.h"
#include "sensordata.h"

namespace Ui {
class CesiumWebView;
}

class CesiumWebView : public QWebView
{
    Q_OBJECT

public:
    CesiumWebView(QWidget *parent = 0);

signals:
    void sendSensorData(QVector<SensorData> &data);

private:
    void dropEvent(QDropEvent *event);
    void drawDataToChart(QVector<SensorData> &data);
};

#endif // CESIUMWEBVIEW_H

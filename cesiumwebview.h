#ifndef CESIUMWEBVIEW_H
#define CESIUMWEBVIEW_H

#include <QWidget>
#include <QWebView>
#include <QDropEvent>

#include "converter.h"
#include "qcustomplot.h"

namespace Ui {
class CesiumWebView;
}

class CesiumWebView : public QWebView
{
    Q_OBJECT

public:
    CesiumWebView(QWidget *parent = 0);

private:
    void dropEvent(QDropEvent *event);
    void drawDataToChart(QVector<SensorData> &data);
};

#endif // CESIUMWEBVIEW_H

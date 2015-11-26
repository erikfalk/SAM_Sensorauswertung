#ifndef CESIUMWEBVIEW_H
#define CESIUMWEBVIEW_H

#include <QWidget>
#include <QWebView>
#include <QDropEvent>

namespace Ui {
class CesiumWebView;
}

class CesiumWebView : public QWebView
{
    Q_OBJECT

public:
    CesiumWebView(QWidget *parent = 0);

protected:
    void dropEvent(QDropEvent *event);


};

#endif // CESIUMWEBVIEW_H

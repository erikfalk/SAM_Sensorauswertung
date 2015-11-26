#include "cesiumwebview.h"
#include <QDebug>

CesiumWebView::CesiumWebView(QWidget *parent) : QWebView(parent) {

}

void CesiumWebView::dropEvent(QDropEvent *event){

   QWebView::dropEvent(event);

   qDebug() << "put data in plot";

}







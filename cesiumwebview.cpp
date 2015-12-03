#include <QDebug>
#include <QMimeData>

#include "cesiumwebview.h"
#include "converter.h"

CesiumWebView::CesiumWebView(QWidget *parent) : QWebView(parent) {

}

void CesiumWebView::dropEvent(QDropEvent *event){

  QWebView::dropEvent(event);

  //get filepath of draged file
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasUrls()) {
    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();
    Converter czmlConverter;

    // extract the local paths of the files
    for (int i = 0; i < urlList.size(); i++) {
        pathList.append(urlList.at(i).toLocalFile());
    }
    czmlConverter.readCzml(pathList.at(0));
   }




}







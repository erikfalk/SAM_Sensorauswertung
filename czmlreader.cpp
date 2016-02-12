#include "czmlreader.h"

CzmlReader::CzmlReader()
{

}

QVector<SensorData> CzmlReader::read(QString filename)
{
    QVector<SensorData> sensordatas;

    //file opening
    QFile czmlFile;
    QString czmlString;
    czmlFile.setFileName(filename);
    if(!czmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        //QMessageBox::warning(this, "Warning!", "Can´t read file!");
        qDebug() << "could not open file for writing";
        return sensordatas;
    }
    czmlString = czmlFile.readAll();
    czmlFile.close();

    //json
    QJsonDocument czmlDoc = QJsonDocument::fromJson(czmlString.toUtf8());
    QJsonArray czmlArray = czmlDoc.array();

    for(int i = 1; i < czmlArray.size(); i++){
        sensordatas.append(writeToSensorData(czmlArray[i].toObject()));
    }

    return sensordatas;
}

SensorData CzmlReader::writeToSensorData(QJsonObject& data)
{
    QJsonArray positionArray;
    QJsonObject positionObject;
    SensorData sensordata;

    QString id = data["id"].toString();
    sensordata.setId(getIdFromCzmlString(id));

    positionObject = data["position"].toObject();
    positionArray = positionObject["cartographicDegrees"].toArray();
    QGeoCoordinate parsedPosition;
    parsedPosition.setLatitude(positionArray[1].toDouble());
    parsedPosition.setLongitude(positionArray[0].toDouble());
    sensordata.setPosition(parsedPosition);
    sensordata.setHeight(positionArray[2].toDouble());

    sensordata.setSensorValue(data["sensorvalue"].toDouble());

    return sensordata;
}

long CzmlReader::getIdFromCzmlString(QString idString){
    QRegExp numberFilter("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
    numberFilter.indexIn(idString);
    QStringList idList = numberFilter.capturedTexts();
    if(idList.isEmpty())
        return -1;

    return idList.begin()->toLong();
}

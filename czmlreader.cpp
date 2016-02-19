#include "czmlreader.h"

CzmlReader::CzmlReader()
{

}

void CzmlReader::read(QString filename, QVector<SensorData>& data)
{
    //file opening
    QFile czmlFile;
    QString czmlString;
    czmlFile.setFileName(filename);
    if(!czmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
       return;
    }

    czmlString = czmlFile.readAll();
    czmlFile.close();

    //json
    QJsonDocument czmlDoc = QJsonDocument::fromJson(czmlString.toUtf8());
    QJsonArray czmlArray = czmlDoc.array();

    for(int i = 1; i < czmlArray.size(); i++){
        QJsonObject czmlObject = czmlArray.at(i).toObject();
        data.append(writeToSensorData(czmlObject));
    }
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

    if(data["sensorvalue"].isString())
        sensordata.setSensorValue(std::numeric_limits<double>::min());
    else
        sensordata.setSensorValue(data["sensorvalue"].toDouble());

    return sensordata;
}

long CzmlReader::getIdFromCzmlString(QString idString)
{
    QRegExp numberFilter("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
    numberFilter.indexIn(idString);
    QStringList idList = numberFilter.capturedTexts();
    if(idList.isEmpty())
        return -1;

    return idList.begin()->toLong();
}

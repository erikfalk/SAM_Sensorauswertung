#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QtMath>

#include "grubbsdata.h"
#include "converter.h"
#include "specialfunctions.h"
#include "converter.h"


Converter::~Converter()
{

}

//creates a czml file
int Converter::writeCzml (QDir filePath, const QVector<SensorData>& data){
    QDateTime time;
    QString fileName = time.currentDateTime().toString("ss_ddMMyyyy") + ".czml";

    QFile czmlFile(filePath.absolutePath() + "/" + fileName);
    

    //open file for writing
    if(!czmlFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for writing";
        return -1;
    }

    QTextStream czmlData(&czmlFile);
    //start
    czmlData << "[\r\n {\r\n  \"id\":\"document\", \r\n  \"version\":\"1.0\"\r\n }";

    //write points
    for(int i = 0; i < data.length(); i++){

        //get point color
        QColor color = mapValueToColor(data[i].getSensorValue());


        czmlData << ",\n{\n \"id\": \"Monitoring point: " << data[i].getId() << "\",\n"
                    " \"description\": \"Position: " << data[i].getPosition().toString(QGeoCoordinate::DegreesWithHemisphere) << " \",\n"
                    " \"availability\": \"" << data[i].getDateTime().toString(Qt::ISODate) << "Z/"
                                           << _latestDateTime.toString(Qt::ISODate) << "Z\",\n"
                    " \"point\":{\n"
                    "  \"color\":{\n"
                    "   \"rgba\":[" << color.red() << "," << color.green() << "," << color.blue() << ",255]\n"
                    " },\n";


        czmlData << " \"outlineColor\":{\n"
                    "  \"rgba\": [" << color.red() << "," << color.green() << "," << color.blue() << ",255]\n"
                    " },\n"
                    " \"outlineWidth\":5,\n"
                    " \"pixelsize\": 200.0,\n"
                    " \"show\": true\n"
                    " },\n";

        czmlData << " \"position\":{\n"
                    "  \"cartographicDegrees\":[" << data[i].getPosition().longitude() << ", "
                                             << data[i].getPosition().latitude() << ", 0]\n"
                    " },\n";


        czmlData << " \"sensorvalue\": " << data[i].getSensorValue() << "\n}";
    }

    //end
    czmlData << "]";

    czmlFile.flush();
    czmlFile.close();

    return 0;
}

//returns a color, between red and green, corrosponding to the sensorvalue
QColor Converter::mapValueToColor(double sensorValue){

    //calculate hue
    double hue = (_maxSensorValue-sensorValue)/((_maxSensorValue-_minSensorValue)*3);

    //calculate corrosponding color
    QColor color = QColor::fromHsvF(hue, 1.0, 1.0);

    return color.toRgb();
}

void Converter::findMinMaxSensorValue() {

    double sensorValue;
    for(int i = 0; i < _completeSensorData.count(); i++){
        sensorValue = _completeSensorData.at(i).getSensorValue();

        if(sensorValue >= _maxSensorValue)
            _maxSensorValue = sensorValue;

        if(sensorValue <= _minSensorValue)
            _minSensorValue = sensorValue;
    }
}

//read data from czml file and put it into a Vector
int Converter::readCzml(QString filename ,QVector<SensorData> &readSensorData){

    //file opening
    QFile czmlFile;
    QString czmlString;
    czmlFile.setFileName(filename);
    if(!czmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    czmlString = czmlFile.readAll();
    czmlFile.close();


    //json
    QJsonDocument czmlDoc = QJsonDocument::fromJson(czmlString.toUtf8());
    QJsonArray czmlArray = czmlDoc.array(), positionArray;
    QJsonObject czmlObject, positionObject;
    SensorData sensordata;

    for(int i = 1; i < czmlArray.size(); i++){

        czmlObject = czmlArray[i].toObject();
        QString id = czmlObject["id"].toString();
        sensordata.setId(getIdFromCzmlString(id));

        positionObject = czmlObject["position"].toObject();
        positionArray = positionObject["cartographicDegrees"].toArray();
        QGeoCoordinate parsedPosition;
        parsedPosition.setLatitude(positionArray[1].toDouble());
        parsedPosition.setLongitude(positionArray[0].toDouble());
        sensordata.setPosition(parsedPosition);
        sensordata.setHeight(positionArray[2].toDouble());

        sensordata.setSensorValue(czmlObject["sensorvalue"].toDouble());

        readSensorData.append(sensordata);
    }

    return 0;
}


long Converter::getIdFromCzmlString(QString idString){
    QRegExp numberFilter("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
    numberFilter.indexIn(idString);
    QStringList idList = numberFilter.capturedTexts();
    if(idList.isEmpty())
        return -1;

    return idList.begin()->toLong();
}

//setter and getter
void Converter::setCompleteSensorData(QVector<SensorData> complete){
    _completeSensorData = complete;
}

void Converter::setMaxSensorValue(double value){
    _maxSensorValue = value;
}

void Converter::setMinSensorValue(double value){
    _minSensorValue = value;
}

void Converter::setMaxVehicleSpeed(double speed){
    _maxVehicleSpeed = speed;
}

QVector<SensorData>& Converter::getCompleteSensorData() {
    return _completeSensorData;
}


double Converter::getMaxSensorValue() const{
    return _maxSensorValue;
}

double Converter::getMinSensorValue() const{
    return _minSensorValue;
}

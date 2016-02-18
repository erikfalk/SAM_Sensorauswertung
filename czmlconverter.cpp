#include "czmlconverter.h"

CzmlConverter::CzmlConverter() : _minSensorValue(std::numeric_limits<double>::max()),
                                 _maxSensorValue(std::numeric_limits<double>::min())
{

}

int CzmlConverter::convertToFile(QDir filePath, const QVector<SensorData> &data)
{
    QDateTime time;
    QFile czmlFile;

    int i = 1;
    do {

        QString fileName = time.currentDateTime().toString("ddMMyyyy") + "_" + QString::number(i) +".czml";
        czmlFile.setFileName(filePath.absolutePath() + "/" + fileName);
        i++;
    }
    while(czmlFile.exists());

    //open file for writing
    if(!czmlFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for writing";
        return -1;
    }

    findExtrema(data);
    qDebug() << _latestDateTime;

    QTextStream czmlData(&czmlFile);
    //start
    czmlData << "[\r\n {\r\n  \"id\":\"document\", \r\n  \"version\":\"1.0\"\r\n }";

    //write points
    for(int i = 0; i < data.length(); i++){

        //get point color
        QColor color = mapValueToColor(data[i].getSensorValue());


        czmlData << ",\n{\n \"id\": \"Monitoring point: " << data[i].getId() << "\",\n"
                   " \"description\": \"Position:    " << data[i].getPosition().toString(QGeoCoordinate::DegreesWithHemisphere) << "<br>";

        czmlData << "Height: " << data[i].getHeight() << "<br>";

        if(data[i].getSensorValue() != std::numeric_limits<double>::min())
            czmlData <<                    "Sensorvalue: " << data[i].getSensorValue() << "<br>";
        else
            czmlData <<                    "Sensorvalue: n/a <br>";

        czmlData <<                    "ISO Date:    " << data[i].getDateTime().toString(Qt::ISODate) << "\",\n"
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
                                             << data[i].getPosition().latitude() << ", " <<  data[i].getHeight() << "]\n"
                    " },\n";

        if(data[i].getSensorValue() != std::numeric_limits<double>::min())
            czmlData << " \"sensorvalue\": " << data[i].getSensorValue() << "\n}";
        else
            czmlData << " \"sensorvalue\": \"n/a\" ""\n}";
    }

    //end
    czmlData << "]";

    czmlFile.flush();
    czmlFile.close();

    return 0;
}

void CzmlConverter::findExtrema(const QVector<SensorData> &data)
{
    QDateTime latestDateTime = data.at(0).getDateTime();

    for(int i = 0; i < data.count(); i++){
        double sensorvalue = data.at(i).getSensorValue();

        if(sensorvalue < _minSensorValue)
            _minSensorValue = sensorvalue;

        if(sensorvalue > _maxSensorValue)
            _maxSensorValue = sensorvalue;

        if(data.at(i).getDateTime() >= latestDateTime)
            _latestDateTime = data.at(i).getDateTime();
    }
}

QColor CzmlConverter::mapValueToColor(double sensorValue)
{
    QColor color;
    if(sensorValue != std::numeric_limits<double>::min()){
        //calculate hue
        double hue = (_maxSensorValue-sensorValue)/((_maxSensorValue-_minSensorValue)*3);

        //calculate corrosponding color
        color = QColor::fromHsvF(hue, 1.0, 1.0);
    } else {
        color = QColor::fromRgb(127, 255, 240);
    }
    return color.toRgb();
}


double CzmlConverter::getMinSensorValue() const
{
    return _minSensorValue;
}

double CzmlConverter::getMaxSensorValue() const
{
    return _maxSensorValue;
}

QDateTime CzmlConverter::getLatestDateTime() const
{
    return _latestDateTime;
}








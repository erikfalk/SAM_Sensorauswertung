#include "czmlconverter.h"

CzmlConverter::CzmlConverter() : _minSensorValue(std::numeric_limits<double>::max()),
                                 _maxSensorValue(std::numeric_limits<double>::min())
{

}

int CzmlConverter::convertToFile(QDir filePath, const QVector<SensorData> &data)
{


    QDateTime time;
    QString fileName = time.currentDateTime().toString("ss_ddMMyyyy") + ".czml";
    QFile czmlFile(filePath.absolutePath() + "/" + fileName);

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
    //calculate hue
    double hue = (_maxSensorValue-sensorValue)/((_maxSensorValue-_minSensorValue)*3);

    //calculate corrosponding color
    QColor color = QColor::fromHsvF(hue, 1.0, 1.0);

    return color.toRgb();
}




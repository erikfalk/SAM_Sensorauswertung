#ifndef CZMLCONVERTER_H
#define CZMLCONVERTER_H

#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QTextStream>

#include "converter.h"

class CzmlConverter : public Converter
{

private:

    QDateTime _latestDateTime;
    double _minSensorValue, _maxSensorValue;

    //returns the a color corrosponding to the value
    QColor mapValueToColor(double sensorValue);

public:
    CzmlConverter();    
    //creates a czml file from data stored in the vector
    int convertToFile(QDir filePath, const QVector<SensorData>& data);
    //find maximum, minimum sensor value and latest date time
    void findExtrema(const QVector<SensorData> &data);

    double getMinSensorValue() const;
    double getMaxSensorValue() const;
    QDateTime getLatestDateTime() const;

};

#endif // CZMLCONVERTER_H

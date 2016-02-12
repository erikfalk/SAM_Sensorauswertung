#ifndef CSVREADER_H
#define CSVREADER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

#include "reader.h"

class CsvReader : public Reader
{
private:

    QDateTime _latestDateTime;
    double _maxSensorValue, _minSensorValue, _maxVehicleSpeed;

    bool gpsChecksum(QString &dataline);

protected:

    //convertes a line from the csv file into the data object
    virtual SensorData writeToSensorData(long id, QString &rawDataString);

public:

    CsvReader();
    QVector<SensorData> read(QString filename);

};

#endif // CSVREADER_H

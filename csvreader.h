#ifndef CSVREADER_H
#define CSVREADER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QTextStream>

#include "reader.h"

class CsvReader : public Reader
{

private:

    bool gpsChecksum(QString &dataline);

    //convertes a line from the csv file into the data object
    SensorData writeToSensorData (long id, QString &rawDataString);

public:
    CsvReader();
    void read(QString filename);

signals:

};

#endif // CSVREADER_H

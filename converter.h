#ifndef CONVERTER
#define CONVERTER

#include <QStringList>
#include <QFile>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QTextStream>
#include <QDateTime>
#include <QTime>
#include <QDate>
#include <QGeoCoordinate>

#include "gpschecksum.h"

struct SensorData {
    QDateTime dateTime;
    QGeoCoordinate position;
    double sog, cog, height, sensor_value;
};

//This function extract specific Data from a GPS rawdata csv file
int getSensorData(QString filename, QVector<SensorData>& complete, QVector<SensorData>& incomplete);

//This function creates a czml file
int writeCzml (QString filename, QVector<SensorData>& data);

//This function converte the data from the csv file into the struct
SensorData convertString (QString &rawDataString);

//This function checks the checksum of the gps signal

#endif // CONVERTER


#ifndef CONVERTER
#define CONVERTER

#include <QStringList>
#include <QFile>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QTextStream>

struct SensorData {
    int hour, minute, second, day, month, year;
    char orientation_lat; //N or S
    char orientation_lon; //E or W
    double lat, lon, sog, cog; //latitude, longitude, speed and course over ground
};

//This function extract specific Data from a GPS rawdata csv file
int getSensorData(QString filename, QVector<SensorData>& data);

//This function creates a czml file
int writeCzml (QString filename, QVector<SensorData>& data);


#endif // CONVERTER


#ifndef CONVERTER
#define CONVERTER

#include <QStringList>
#include <QFile>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QTextStream>

struct SensorData {
    float lat, lon;
};

//This function extract specific Data from a GPS rawdata csv file
int getSensorData(QString filename, QVector<SensorData>& data);

//This function creates a czml file
int writeCzml (QString filename, QVector<SensorData>& data);


#endif // CONVERTER


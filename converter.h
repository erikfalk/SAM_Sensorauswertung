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
#include <QColor>
#include <QDir>

#include "sensordata.h"


class Converter {

private:
    QVector<SensorData> _completeSensorData, _incompleteSensorData;
    QDateTime _latestDateTime;
    double _maxSensorValue, _minSensorValue, _maxVehicleSpeed;

    //convertes a line from the csv file into the struct
    SensorData convertString (QString &rawDataString);

    //find and removes peaks in data
    void findPeak(QVector<SensorData>& data);

    //returns true if GPS Dataset Checksum is correct
    bool gpsChecksum(QString &dataline);

    //returns the a color corrosponding to the value
    QColor mapValueToColor(double sensorValue);

public:

    Converter() : _maxSensorValue(1.7e-308), _minSensorValue(1.7e+308), _maxVehicleSpeed(20){}
    ~Converter();

    //extract specific Data from a GPS rawdata csv file
    int extractSensorData(QString filename);

    //creates a czml file
    int writeCzml(QDir filePath, const QVector<SensorData>& data);

    //reads a czml file
    int readCzml(QString filename, QVector<SensorData>& readSensorData);

    double findMaxSensorValue(QVector<SensorData> &sensordata);
    double findMinSensorValue(QVector<SensorData> &sensordata);

    //setter
    void setCompleteSensorData(QVector<SensorData> complete);
    void setIncompleteSensorData(QVector<SensorData> incomplete);
    void setMaxSensorValue(double value);
    void setMinSensorValue(double value);
    void setMaxVehicleSpeed(double speed);

    //getter
    const QVector<SensorData>& getCompleteSensorData();
    const QVector<SensorData>& getIncompleteSensorData();
    double getMaxSensorValue() const;
    double getMinSensorValue() const;
    double getMaxVehicleSpeed() const;
};

#endif // CONVERTER


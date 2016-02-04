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
    QVector<SensorData> _completeSensorData;
    QDateTime _latestDateTime;
    double _maxSensorValue, _minSensorValue, _maxVehicleSpeed;

    //convertes a line from the csv file into the data object
    SensorData convertString (long id, QString &rawDataString);

    //find and removes peaks in data
    void findPeak(QVector<SensorData>& data);

    //returns true if GPS Dataset Checksum is correct
    bool gpsChecksum(QString &dataline);

    //returns the a color corrosponding to the value
    QColor mapValueToColor(double sensorValue);

public:

    Converter() : _maxSensorValue(std::numeric_limits<double>::min()), _minSensorValue(std::numeric_limits<double>::max()), _maxVehicleSpeed(20){}
    ~Converter();

    //extract specific Data from a GPS rawdata csv file
    int extractSensorData(QString filename);

    //creates a czml file from data stored in the vector
    int writeCzml(QDir filePath, const QVector<SensorData>& data);

    //reads a czml file and stores data in the vector
    int readCzml(QString filename, QVector<SensorData>& readSensorData);
    long getIdFromCzmlString(QString idString);

    void findMinMaxSensorValue();

    //setter
    void setCompleteSensorData(QVector<SensorData> complete);
    void setMaxSensorValue(double value);
    void setMinSensorValue(double value);
    void setMaxVehicleSpeed(double speed);

    //getter
    QVector<SensorData>& getCompleteSensorData();
    double getMaxSensorValue() const;
    double getMinSensorValue() const;
    double getMaxVehicleSpeed() const;
};

#endif // CONVERTER


#ifndef READER_H
#define READER_H
#include <QString>
#include <QVector>
#include <QObject>
#include "sensordata.h"

class Reader
{

protected:

    virtual SensorData writeToSensorData();

public:

     Reader();
    ~Reader();
    virtual QVector<SensorData> read(QString filename) = 0;

};

#endif // READER_H

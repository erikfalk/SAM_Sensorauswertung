#ifndef READER_H
#define READER_H
#include <QString>
#include <QVector>
#include <QObject>
#include "sensordata.h"

class Reader
{

private:

    virtual SensorData writeToSensorData() = 0;

public:

     Reader();
    ~Reader();
    virtual void read() = 0;

};

#endif // READER_H

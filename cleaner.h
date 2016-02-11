#ifndef CLEANER_H
#define CLEANER_H
#include <QVector>
#include <QObject>
#include <QtMath>

#include "sensordata.h"
#include "grubbsdata.h"

class Cleaner
{
public:
    Cleaner();
    ~Cleaner();

    //find and removes outlier in data
    void grubbsTest(QVector<SensorData>& data);
};

#endif // CLEANER_H

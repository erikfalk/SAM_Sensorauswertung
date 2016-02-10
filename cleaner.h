#ifndef CLEANER_H
#define CLEANER_H
#include <QVector>
#include <QObject>
#include "sensordata.h"

class Cleaner : public QObject
{

private:
    Q_OBJECT

    QVector<SensorData> _dataToClean;

public:
    Cleaner();

    //find and removes outlier in data
    void grubbsTest();

public slots:
    void onSensorDatarecieved(QVector<SensorData>& data);
};

#endif // CLEANER_H

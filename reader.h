#ifndef READER_H
#define READER_H
#include <QString>
#include <QVector>
#include <QObject>
#include "sensordata.h"

class Reader : public QObject
{

private:

    Q_OBJECT

    QVector<SensorData> _readData;

    virtual SensorData writeToSensorData(long id, QString &rawDataString) = 0;

public:
    Reader();
    virtual void read(QString filename) = 0;
    int readFromCzml(QString filename, QVector<SensorData>& readSensorData);


signals:
    void sendSensorData(QVector<SensorData> &data);
};

#endif // READER_H

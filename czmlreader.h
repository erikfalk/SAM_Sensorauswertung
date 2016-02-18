#ifndef CZMLREADER_H
#define CZMLREADER_H

#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QDebug>

#include "sensordata.h"
#include "reader.h"

class CzmlReader : public Reader
{
private:
    long getIdFromCzmlString(QString idString);

protected:
    //convertes a line from the czml file into the data object
    virtual SensorData writeToSensorData (QJsonObject& data);

public:
    CzmlReader();
    virtual void read(QString filename, QVector<SensorData>& data);

};

#endif // CZMLREADER_H

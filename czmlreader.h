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

    //convertes a line from the czml file into the data object
    SensorData writeToSensorData (QJsonObject& data);

public:
    CzmlReader();
    virtual QVector<SensorData> read(QString filename);

};

#endif // CZMLREADER_H

#ifndef CZMLREADER_H
#define CZMLREADER_H

#include <QString>

#include "sensordata.h"
#include "reader.h"

class CzmlReader : public Reader
{
private:
    long getIdFromCzmlString(QString idString);

    //convertes a line from the czml file into the data object
    SensorData writeToSensorData (long id, QString &rawDataString);

public:
    CzmlReader();
    void read(QString filename, QVector<SensorData>& data);
    //void read(QString filename, QVector<SensorData>& data);

};

#endif // CZMLREADER_H

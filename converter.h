#ifndef CONVERTER
#define CONVERTER

#include <QDir>
#include <QVector>
#include <QDateTime>
#include <limits>

#include "sensordata.h"



class Converter {

private:

public:

    Converter();
    ~Converter();

    //creates a czml file from data stored in the vector
    virtual int convertToFile(QDir filePath, const QVector<SensorData>& data) = 0;
    virtual void findExtrema();
};

#endif // CONVERTER


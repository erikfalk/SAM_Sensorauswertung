#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QDateTime>
#include <QGeoCoordinate>


class SensorData
{

private:
    long id;
    QDateTime dateTime;
    QGeoCoordinate position;
    double speedOverGround;
    double courseOverGround;
    double height;
    double sensorValue;

public:
    SensorData(long id = 0, double h = 0.0, double v = 0.0) : id(id), height(h), sensorValue(v){}

    void setId(long id);
    void setDateTime(QDateTime dateTime);
    void setPosition(QGeoCoordinate position);
    void setSpeedOverGround(double sog);
    void setCourseOverGround(double cog);
    void setHeight(double height);
    void setSensorValue(double sensorValue);

    long getId() const;
    QDateTime getDateTime() const;
    QGeoCoordinate getPosition() const;
    double getSpeedOverGround() const;
    double getCourseOverGround() const;
    double getHeight() const;
    double getSensorValue() const;
};

#endif // SENSORDATA_H

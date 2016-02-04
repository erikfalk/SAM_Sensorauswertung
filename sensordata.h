#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QDateTime>
#include <QGeoCoordinate>


class SensorData
{

private:
    QDateTime dateTime;
    QGeoCoordinate position;
    double speedOverGround;
    double courseOverGround;
    double height;
    double sensorValue;
    long id;

public:
    SensorData(double h = 0.0) : height(h){}

    void setDateTime(QDateTime dateTime);
    void setPosition(QGeoCoordinate position);
    void setSpeedOverGround(double sog);
    void setCourseOverGround(double cog);
    void setHeight(double height);
    void setSensorValue(double sensorValue);
    void setID(long id);

    QDateTime getDateTime() const;
    QGeoCoordinate getPosition() const;
    double getSpeedOverGround() const;
    double getCourseOverGround() const;
    double getHeight() const;
    double getSensorValue() const;
    long getID() const;


};

#endif // SENSORDATA_H

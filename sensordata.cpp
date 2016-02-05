#include "sensordata.h"

void SensorData::setId(long id) {
    this->id = id;
}

void SensorData::setDateTime(QDateTime dateTime){
    this->dateTime = dateTime;
}

void SensorData::setPosition(QGeoCoordinate position){
    this->position = position;
}

void SensorData::setSpeedOverGround(double sog){
    speedOverGround = sog;
}

void SensorData::setCourseOverGround(double cog){
    courseOverGround = cog;
}

void SensorData::setHeight(double height){
    this->height = height;
}

void SensorData::setSensorValue(double sensorValue){
    this->sensorValue = sensorValue;
}

long SensorData::getId() const {
    return id;
}

QDateTime SensorData::getDateTime() const{
    return dateTime;
}

QGeoCoordinate SensorData::getPosition() const{
    return position;
}

double SensorData::getSpeedOverGround() const{
    return speedOverGround;
}

double SensorData::getCourseOverGround() const{
    return courseOverGround;
}

double SensorData::getHeight() const{
    return height;
}

double SensorData::getSensorValue() const{
    return sensorValue;
}


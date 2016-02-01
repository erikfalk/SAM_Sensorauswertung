#include "grubbs.h"

Grubbs::Grubbs()
{

}


void Grubbs::setDataID(long dataID)
{
    _dataID = dataID;
}

void Grubbs::setSpeed(double speed)
{
    _speed = speed;
}

void Grubbs::setG(double g)
{
    _G = g;
}


long Grubbs::getDataID() const
{
    return _dataID;
}

double Grubbs::getSpeed() const
{
    return _speed;
}

double Grubbs::getG() const
{
    return _G;
}


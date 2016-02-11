#include "grubbsdata.h"

GrubbsData::GrubbsData()
{

}

void GrubbsData::setSpeed(double speed)
{
    _speed = speed;
}

void GrubbsData::setG(double g)
{
    _G = g;
}

double GrubbsData::getSpeed() const
{
    return _speed;
}

double GrubbsData::getG() const
{
    return _G;
}


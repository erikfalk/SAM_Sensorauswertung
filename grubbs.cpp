#include "grubbs.h"

Grubbs::Grubbs()
{

}

void Grubbs::setSpeed(double speed)
{
    _speed = speed;
}

void Grubbs::setG(double g)
{
    _G = g;
}

double Grubbs::getSpeed() const
{
    return _speed;
}

double Grubbs::getG() const
{
    return _G;
}


#ifndef GRUBBSDATA_H
#define GRUBBSDATA_H


class GrubbsData
{
    long _id, _dataID;
    double _speed, _G;
public:
    GrubbsData();

    void setSpeed(double speed);
    void setG(double g);

    double getSpeed() const;
    double getG() const;


};

#endif // GRUBBSDATA_H

#ifndef GRUBBS_H
#define GRUBBS_H


class Grubbs
{
    long _id, _dataID;
    double _speed, _G;
public:
    Grubbs();

    void setDataID(long dataID);
    void setSpeed(double speed);
    void setG(double g);

    long getDataID() const;
    double getSpeed() const;
    double getG() const;


signals:

public slots:
};

#endif // GRUBBS_H

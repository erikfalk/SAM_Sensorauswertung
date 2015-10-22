#include "findpeaks.h"
#include <math.h>

void findPeak(QVector<SensorData> &complete){

    int maxSpeed = 20, i = 0;
    long long int tempTime;
    double distance, calcSpeed;

    for(i=0; i<10;i++){


        distance = complete[i].position.distanceTo(complete[i+1].position);

        qDebug() << "Entfernung: " << distance;

        qDebug() << "Zeit: " << complete[i].dateTime.time();
        qDebug() << "Datum: " << complete[i].dateTime.date();

        tempTime = complete[i].dateTime.secsTo(complete[i+1].dateTime);

        qDebug() << "Vergangene Zeit" << tempTime;

        calcSpeed = (distance/tempTime)*3.6;

        if(maxSpeed <= calcSpeed){
            complete.erase(complete[i]);
        }

        /*
        if(calcSpeed <= maxSpeed){
            qDebug() << "passt";
            qDebug() << calcSpeed;
        }else{
            qDebug() << "falsch";
            qDebug() << calcSpeed;
        }
        */

    }
}

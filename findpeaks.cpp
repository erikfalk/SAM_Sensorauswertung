#include "findpeaks.h"
#include <math.h>

void findPeak(QVector<SensorData> &data){


    long long int tempTime;
    double distance, calcSpeed;
    int maxSpeed, i;

    for(i=0; i<10;i++){


        distance = data[i].position.distanceTo(data[i+1].position);

        qDebug() << "Entfernung: " << distance;

        qDebug() << "Zeit: " << data[i].dateTime.time();
        qDebug() << "Datum: " << data[i].dateTime.date();

        tempTime = data[i].dateTime.secsTo(data[i+1].dateTime);

        qDebug() << "Vergangene Zeit" << tempTime;

        calcSpeed = (distance/tempTime)*3.6;

        if(maxSpeed <= calcSpeed){
            data.remove(i);
        }

    }
}

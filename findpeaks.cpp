#include "findpeaks.h"
#include <math.h>

void findPeak(QVector<SensorData> &complete){

    int maxSpeed = 20, i = 0, time;
    double dx, dy, distance, calcSpeed;

    for(i=0; i<complete.count();i++){

        dx = 71.5 * (complete[i].lon - complete[i+1].lon);
        dy = 111.3 * (complete[i].lat - complete[i+1].lat);

        distance = sqrt((dx*dx)+(dy*dy));

        qDebug() << " Entfernung: " << distance;
        qDebug() << " Stunde: " << complete[i].hour;
        qDebug() << " Minute: " << complete[i].minute;
        qDebug() << " Sekunde: " << complete[i].second;

        qDebug() << complete[i].time.secsTo(complete[i+1].time);
        qDebug() << time2.secsTo(time1);
            return 0;

        qDebug() << " Zeit: " << time;

        calcSpeed = ((distance*1000) / time)*3.6;

        if()

    }
}
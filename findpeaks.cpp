#include "findpeaks.h"
#include <math.h>

void findPeak(QVector<SensorData> &complete){

    int maxSpeed = 20, i = 0, time;
    double dx, dy, distance;

    for(i=0; i<complete.count();i++){

        dx = 71.5 * (complete[i].lon - complete[i+1].lon);
        dy = 111.3 * (complete[i].lat - complete[i+1].lat);

        distance = sqrt((dx*dx)+(dy*dy));

        qDebug() << " Entfernung: " << distance;
        qDebug() << " Stunde: " << complete[i].hour;
        qDebug() << " Minute: " << complete[i].minute;
        qDebug() << " Sekunde: " << complete[i].second;
        time = ((complete[i+1].hour*3600)+(complete[i+1].minute*60)+complete[i+1].second) - ((complete[i].hour*3600)+(complete[i].minute*60)+complete[i].second);

        qDebug() << " Zeit: " << time;

    }
}

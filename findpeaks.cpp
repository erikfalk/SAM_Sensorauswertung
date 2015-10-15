#include "findpeaks.h"
#include <math.h>

void findPeak(QVector<SensorData> &complete){

    int maxSpeed = 20, i = 0;
    double dx, dy, distance;

    for(i=0; i<complete.count();i++){

        dx = 71.5 * (complete[i].lon - complete[i+1].lon);
        dy = 111.3 * (complete[i].lat - complete[i+1].lat);

        distance = sqrt((dx*dx)+(dy*dy));




    }
}

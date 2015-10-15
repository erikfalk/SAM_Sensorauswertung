#include "findpeaks.h"
#include <math.h>

void findPeak(QVector<SensorData> &complete){

    int maxSpeed = 20, i = 0, time;
    double dx, dy, distance;

    for(i=0; i<complete.count();i++){

        dx = 71.5 * (complete[i].lon - complete[i+1].lon);
        dy = 111.3 * (complete[i].lat - complete[i+1].lat);

        distance = sqrt((dx*dx)+(dy*dy));

        qDebug()<<distance;

        if(complete[i+1].second < complete[i].second){
            if(complete[i+1].minute < complete[i].minute){
                time = (((60-complete[i].minute)+complete[i+1].minute)*60) + ((60-complete[i].second)+complete[i+1].second);
            } else {
                time = ((complete[i+1].minute - complete[i].minute)*60) + ((60-complete[i].second)+complete[i+1].second);
            }
        } else {
            if(complete[i+1].minute < complete[i].minute){
                time = (((60-complete[i].minute)+complete[i+1].minute)*60) + (complete[i+1].second - complete[i].second);
            }
        }

    }
}

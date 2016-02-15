#include "cleaner.h"

Cleaner::Cleaner()
{

}

Cleaner::~Cleaner()
{

}

void Cleaner::grubbsTest(QVector<SensorData>& data)
{
    //Prüfen ob leer noch hinzufügen

    long int millisecsToPoint;
    long degreeOfFreedom, toDeleteVariable = -1, outlierDataCount;
    double timeToPoint, distance, calcSpeed, heightDif, max = 0, tCrit, gCrit, mean, sum = 0, stdev, numerator = 0, sigValue, g;
    bool foundOutlier;
    GrubbsData outlierSearchData;


    do{
        QVector<GrubbsData> outlierSearchDataVector;

        //read gpsdata and calculate speed between two points and save it in vector
        for(int i=0; i < data.count()-1; i++){

            distance = data[i].getPosition().distanceTo(data[i+1].getPosition());
            heightDif = qFabs(data[i+1].getHeight() - data[i].getHeight());
            millisecsToPoint = data[i].getDateTime().msecsTo(data[i+1].getDateTime());

            if(data[i].getId()<100)
                qDebug() << "Line ID: " << data[i].getId() << " Position: " << data[i].getPosition();

            //change duration in seconds
            timeToPoint = millisecsToPoint / 1000.0;

            //speed in m/s
            if(timeToPoint == 0.0){
                calcSpeed = 0.0;
            } else {
                calcSpeed = ( qSqrt( qPow(heightDif, 2) + qPow(distance, 2 ) ) ) / timeToPoint;
            }

            //add data to vector
            outlierSearchData.setSpeed(calcSpeed);
            outlierSearchDataVector.append(outlierSearchData);
        }

        outlierDataCount = outlierSearchDataVector.count();

        //calculate sum of speed from all dataset
        for(int i=0; i < outlierDataCount-1; i++){
            sum += outlierSearchDataVector[i].getSpeed();
        }

        //calculate average over all values
        mean = sum / outlierDataCount;

        //calculate numerator for standard deviation
        for(int i=0; i < outlierDataCount-1; i++){
            numerator += qPow((outlierSearchDataVector[i].getSpeed() - mean),2);
        }

        //calculate standard deviation
        stdev = qSqrt(numerator / outlierDataCount);

        //define significant value
        sigValue = 0.05 / outlierDataCount;

        //define degrees of Freedom for t-distribution
        degreeOfFreedom = outlierDataCount-2;

        //calculate t-distribution -- need to calculate gCrit
        tCrit = qFabs(alglib::invstudenttdistribution(degreeOfFreedom, sigValue));

        //calculate gCrit -- all points with a G value over gCrit are outliers
        gCrit = (outlierDataCount-1)*tCrit/qSqrt(outlierDataCount*((outlierDataCount-2)+qPow(tCrit, 2)));

        //calculate G for all values and find the dataset with the highest G value over gCrit
        for(int i=0; i < outlierDataCount-1; i++){
            g = qFabs((mean - outlierSearchDataVector[i].getSpeed()) / stdev);
            outlierSearchDataVector[i].setG(g);

            if(gCrit < g){
                if(max == 0){
                    max = g;
                    toDeleteVariable = i+1;
                } else if(max < g){
                    max = g;
                    toDeleteVariable = i+1;
                }
            }
        }

        foundOutlier = false;

        // delete the dataset with the highest G value over gCrit
        if(toDeleteVariable != -1){

            data.remove(toDeleteVariable);

            foundOutlier = true;
            toDeleteVariable = -1;
        }

    } while (foundOutlier);

}


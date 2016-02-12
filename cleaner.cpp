#include "cleaner.h"

Cleaner::Cleaner()
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
    QVector<GrubbsData> outlierSearchDataVector;

    //read gpsdata and calculate speed between two points and save it in vector
    qDebug() << "Geschwindigkeitsberechnung";
    for(int i=0; i < data.count()-1; i++){

        distance = data[i].getPosition().distanceTo(data[i+1].getPosition());
        heightDif = qFabs(data[i+1].getHeight() - data[i].getHeight());
        millisecsToPoint = data[i].getDateTime().msecsTo(data[i+1].getDateTime());

        //qDebug() << "milliseconds: " << millisecsToPoint;

        //change duration in seconds
        timeToPoint = millisecsToPoint / 1000.0;

        //speed in m/s
        if(timeToPoint == 0.0){
            calcSpeed = 0.0;
        } else {
            calcSpeed = ( qSqrt( qPow(heightDif, 2) + qPow(distance, 2 ) ) ) / timeToPoint;
        }

        //qDebug() << "Daten ID: " << i << " Geschwindigkeit: " << calcSpeed << " heightDif " << heightDif << " distance: " << distance << "seconds" << timeToPoint;

        //add data to vector
        outlierSearchData.setSpeed(calcSpeed);
        outlierSearchDataVector.append(outlierSearchData);
    }

    do{
        outlierDataCount = outlierSearchDataVector.count();

        //calculate sum of speed from all dataset
        for(int i=0; i < outlierDataCount-1; i++){
            sum += outlierSearchDataVector[i].getSpeed();
        }

        //calculate average over all values
        mean = sum / outlierDataCount;
        qDebug() << "mittelwert: " << mean;

        //calculate numerator for standard deviation
        qDebug() << "Zähler bestimmen";
        for(int i=0; i < outlierDataCount-1; i++){
            numerator += qPow((outlierSearchDataVector[i].getSpeed() - mean),2);
        }

        //calculate standard deviation
        stdev = qSqrt(numerator / outlierDataCount);
        qDebug() << "standardverteilung: " << stdev;

        //define significant value
        sigValue = 0.05 / outlierDataCount;

        //define degrees of Freedom for t-distribution
        degreeOfFreedom = outlierDataCount-2;

        //calculate t-distribution -- need to calculate gCrit
        tCrit = qFabs(alglib::invstudenttdistribution(degreeOfFreedom, sigValue));

        //calculate gCrit -- all points with a G value over gCrit are outliers
        gCrit = (outlierDataCount-1)*tCrit/qSqrt(outlierDataCount*((outlierDataCount-2)+qPow(tCrit, 2)));
        qDebug() << "Anzahl: " << outlierDataCount;
        qDebug() << "tCrit: " << tCrit;
        qDebug() << "cGrit: " << gCrit;

        //calculate G for all values and find the dataset with the highest G value over gCrit
        qDebug() << "G berechnen";
        for(int i=0; i < outlierDataCount-1; i++){
            g = qFabs((mean - outlierSearchDataVector[i].getSpeed()) / stdev);
            outlierSearchDataVector[i].setG(g);

            //qDebug() << "Daten ID: " << i << " Geschwindigkeit: " << outlierSearchDataVector[i].getSpeed() << " G: " << g;

            if(gCrit < g){
                if(max == 0){
                    max = g;
                    toDeleteVariable = i;
                } else if(max < g){
                    max = g;
                    toDeleteVariable = i;
                }
            }
        }

        foundOutlier = false;
        // delete the dataset with the highest G value over gCrit
        if(toDeleteVariable != -1){
            qDebug() << "Daten mit ID: " << toDeleteVariable << "wurde geloescht";
            outlierSearchDataVector.remove(toDeleteVariable);
            data.remove(toDeleteVariable);
            foundOutlier = true;
            toDeleteVariable = -1;
        }

    } while (foundOutlier);

}


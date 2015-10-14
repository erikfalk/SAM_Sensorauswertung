#include "converter.h"

//This function extract specific Data from a GPS rawdata csv file
int getSensorData(QString filename, QVector<SensorData>& data){

    QFile rawDataFile(filename);
    QTextStream rawData(&rawDataFile);
    QStringList list;

    //open file for reading
    if(!rawDataFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "could not open file for reading";
        return -1;
    }

    //add a line to the stringlist
    while(!rawData.atEnd()){
       SensorData temp;
       QString line = rawData.readLine();

       //split line into tokens and store in list
       list = line.split(",");

       //Fehlerkorrektur
       //if(line.count() != 13)
           //schreibe in Fehlervector

       //claculate lat and lon
       QString deg;
       QString min;

       //build lat
       //extract degrees
       deg.append(list[3].at(0));
       deg.append(list[3].at(1));
       min = list[3].remove(0,2);
       //calculate degrees
       temp.lat = deg.toDouble() + ((min.toDouble())/60);

       //build lon
       //extract degrees
       deg.clear();
       deg.append(list[5].at(0));
       deg.append(list[5].at(1));
       deg.append(list[5].at(2));
       min = list[5].remove(0,3);
       //calculate degrees
       temp.lon = deg.toDouble() + ((min.toDouble())/60);

       //extract time
       temp.hour   = QString(QString(list[1].at(0)) + QString(list[1].at(1))).toInt();
       temp.minute = QString(QString(list[1].at(2)) + QString(list[1].at(3))).toInt();
       temp.second = QString(QString(list[1].at(4)) + QString(list[1].at(5))).toInt();

       //extract date
       temp.hour   = QString(QString(list[9].at(0)) + QString(list[9].at(1))).toInt();
       temp.minute = QString(QString(list[9].at(2)) + QString(list[9].at(3))).toInt();
       temp.second = QString(QString(list[9].at(4)) + QString(list[9].at(5))).toInt();

       //extract speed and course over ground
       temp.sog = list[8].toDouble();
       temp.cog = list[9].toDouble();

       //check for and add extra data
       if(list.count() > 13){
           //extract height
           bool *ok = NULL;
           list[13].toDouble(ok);
           if(ok)
            temp.height = list[13].toDouble();

           //extract sensor value
           list[14].toDouble(ok);
           if(ok)
            temp.sensor_value = list[14].toDouble(ok);
       }

       data.append(temp);
     }

    rawDataFile.flush();
    rawDataFile.close();

    return 0;
}

//This function creates a czml file
int writeCzml (QString filename, QVector<SensorData>& data){

    QFile czmlFile(filename);

    //open file for writing
    if(!czmlFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for writing";
        return -1;
    }

    QTextStream czmlData(&czmlFile);
    //start
    czmlData << "[{\"id\":\"document\", \"version\":\"1.0\"}\n";

    //write points
    for(int i = 0; i < data.length(); i++){

        czmlData << ",{\"id\": \"Point_Test_" << i << "\",\n"
                    "\"availability\": \"2015-09-22T12:00:00Z/2015-09-22T13:00:00Z\",\n"
                    "\"point\":{\n"
                    "\"color\":{\n"
                    "\"interval\":\"2015-09-22T12:00:00Z/2015-09-22T13:00:00Z\",\n"
                    "\"rgba\":[255,0,0,255]\n"
                    "},\n"
                    "\"outlineColor\":{\n"
                    "\"rgba\": [255,0,0,255]\n"
                    "},\n"
                    "\"outlineWidth\":5,\n"
                    "\"pixelsize\": 200.0,\n"
                    "\"show\": true\n"
                    "},\n";

     czmlData << "\"position\":{\n"
                 "\"epoch\": \"2015-09-22T12:00:00Z\",\n"
                 "\"cartographicDegrees\":[" << data[i].lon << ", " << data[i].lat << ", 0]}}\n";
    }





    //end
    czmlData << "]";
    czmlFile.flush();
    czmlFile.close();

    return 0;
}

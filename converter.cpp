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

       //build lat and lon
       QString lat = list[3].remove(0,2);

       //put the needed listitems to the datastruct
       temp.lat = (list[3].toFloat())/100;
       temp.lon = (list[5].toFloat())/100;
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

#include <QDebug>
#include "csvreader.h"

CsvReader::CsvReader()
{

}

void CsvReader::read(QString filename, QVector<SensorData>& data)
{
    QFile rawDataFile(filename);
    QTextStream rawData(&rawDataFile);

    //open file for Reader
    if(!rawDataFile.open(QFile::ReadOnly | QFile::Text)){
        return;
    }

    //add data to vector
    long lineId = 0;
    while(!rawData.atEnd()){

       QString line = rawData.readLine();

       if(line.contains("$GPRMC")) {
        lineId++;

        //check for complete dataset
        //if(gpsChecksum(line))
            data.append(writeToSensorData(lineId, line));
       }
     }

    rawDataFile.flush();
    rawDataFile.close();
}

bool CsvReader::gpsChecksum(QString &dataline)
{
    QByteArray datalineBytes = dataline.toUtf8();
    QString recieved_checksum;
    int calc_checksum = 0x00;

    for(int i = 0; i < datalineBytes.length(); i++){

            switch(datalineBytes[i]){

                case '$': break;

                case '*': {
                    //extract recieved checksum
                    recieved_checksum.append(datalineBytes.at(i+1));
                    recieved_checksum.append(datalineBytes.at(i+2));
                    i = datalineBytes.length();
                    break;
                }

                default: {
                    calc_checksum ^= datalineBytes[i];
                }
            }
    }

    bool ok;
    if(calc_checksum == recieved_checksum.toInt(&ok,16))
        return true;

    return false;
}

SensorData CsvReader::writeToSensorData(long id, QString &rawDataString)
{
    SensorData sensorDataTemp(id);
    QStringList splittedData;

    //split line into tokens and store in splittedData
    splittedData = rawDataString.split(",");

    //extract lat and lon
    QString degrees;
    QString minutes;
    QGeoCoordinate position;

    //build lat
    //extract degrees and minutes
    degrees.append(splittedData[3].at(0));
    degrees.append(splittedData[3].at(1));
    minutes = splittedData[3].remove(0,2);

    //calculate and set degrees
    if(splittedData.at(4) == "N")
        position.setLatitude(degrees.toDouble() + ((minutes.toDouble())/60));
    else
        position.setLatitude(-(degrees.toDouble() + ((minutes.toDouble())/60)));

    //build lon
    //extract degrees and minutes
    degrees.clear();
    degrees.append(splittedData[5].at(0));
    degrees.append(splittedData[5].at(1));
    degrees.append(splittedData[5].at(2));
    minutes = splittedData[5].remove(0,3);

    //calculate degrees
    if(splittedData.at(6) == "E")
        position.setLongitude(degrees.toDouble() + ((minutes.toDouble())/60));
    else
        position.setLongitude(-(degrees.toDouble() + ((minutes.toDouble())/60)));

    sensorDataTemp.setPosition(position);

    //extract time
    QTime time;
    if((splittedData.at(1)).count() > 6)
         time = QTime::fromString(splittedData.at(1), "hhmmss.zzz");
    else
        time = QTime::fromString(splittedData.at(1), "hhmmss");

    //extract date
    QDate date = QDate::fromString(splittedData.at(9), "ddMMyy");
    date = date.addYears(100);

    sensorDataTemp.setDateTime(QDateTime(date, time));

    //extract and set speed and course over ground
    sensorDataTemp.setSpeedOverGround(splittedData[8].toDouble());
    sensorDataTemp.setCourseOverGround(splittedData[9].toDouble());

    //check for and add extra data
    if(splittedData.count() > 13){
        //extract and set height
        sensorDataTemp.setHeight(splittedData[13].toDouble());

        //extract sensor value
        sensorDataTemp.setSensorValue(splittedData[14].toDouble());
    }

    return sensorDataTemp;
}

CsvReader::~CsvReader()
{

}





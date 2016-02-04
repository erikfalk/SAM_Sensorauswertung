#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QtMath>

#include "grubbs.h"
#include "converter.h"
#include "specialfunctions.h"

Converter::~Converter()
{

}

//This function extract specific Data from a GPS rawdata csv file
int Converter::extractSensorData(QString filename){

    QFile rawDataFile(filename);
    QTextStream rawData(&rawDataFile);

    //open file for reading
    if(!rawDataFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "could not open file for reading";
        return -1;
    }

    //add data to vector
    while(!rawData.atEnd()){

       QString line = rawData.readLine();

       //check for complete dataset
       if(gpsChecksum(line)){
           _completeSensorData.append(convertString(line));
       }else{
           _incompleteSensorData.append(convertString(line));
       }
     }

    rawDataFile.flush();
    rawDataFile.close();

    //find and remove peaks
    findPeak(_incompleteSensorData);

    return 0;
}

//returns a color, between red and green, corrosponding to the sensorvalue
QColor Converter::mapValueToColor(double sensorValue){

    //calculate hue
    double hue = (_maxSensorValue-sensorValue)/((_maxSensorValue-_minSensorValue)*3);

    //calculate corrosponding color
    QColor color = QColor::fromHsvF(hue, 1.0, 1.0);

    return color.toRgb();
}

//extract specific Data from a GPS rawdata csv file
SensorData Converter::convertString(QString &rawDataString){

    SensorData sensorDataTemp;
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
    position.setLatitude(degrees.toDouble() + ((minutes.toDouble())/60));

    //build lon
    //extract degrees and minutes
    degrees.clear();
    degrees.append(splittedData[5].at(0));
    degrees.append(splittedData[5].at(1));
    degrees.append(splittedData[5].at(2));
    minutes = splittedData[5].remove(0,3);
    //calculate degrees
    position.setLongitude(degrees.toDouble() + ((minutes.toDouble())/60));

    sensorDataTemp.setPosition(position);

    //extract time
    QTime time(QString(QString(splittedData[1].at(0)) + QString(splittedData[1].at(1))).toInt(),
               QString(QString(splittedData[1].at(2)) + QString(splittedData[1].at(3))).toInt(),
               QString(QString(splittedData[1].at(4)) + QString(splittedData[1].at(5))).toInt());

    //extract date
    QDate date(2000 + QString(QString(splittedData[9].at(4)) + QString(splittedData[9].at(5))).toInt(),
               QString(QString(splittedData[9].at(2)) + QString(splittedData[9].at(3))).toInt(),
               QString(QString(splittedData[9].at(0)) + QString(splittedData[9].at(1))).toInt());


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

        //extract minimum and maximum sensor value
        if(sensorDataTemp.getSensorValue() > _maxSensorValue)
            _maxSensorValue = sensorDataTemp.getSensorValue();

        if(sensorDataTemp.getSensorValue() < _minSensorValue)
            _minSensorValue = sensorDataTemp.getSensorValue();

        //extract latest date and time
        if(sensorDataTemp.getDateTime() > _latestDateTime)
            _latestDateTime = sensorDataTemp.getDateTime();
    }

    return sensorDataTemp;
}


//creates a czml file
int Converter::writeCzml (QDir filePath, const QVector<SensorData>& data){
    QDateTime time;
    QString fileName = "messung" + time.currentDateTime().toString("ss_ddMMyyyy") + ".czml";

    QFile czmlFile(filePath.absolutePath() + "/" + fileName);
    

    //open file for writing
    if(!czmlFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for writing";
        return -1;
    }

    QTextStream czmlData(&czmlFile);
    //start
    czmlData << "[\r\n {\r\n  \"id\":\"document\", \r\n  \"version\":\"1.0\"\r\n }";

    //write points
    for(int i = 0; i < data.length(); i++){

        //get point color
        QColor color = mapValueToColor(data[i].getSensorValue());


        czmlData << ",\n{\n \"id\": \"Dataset " << i << "\",\n"
                    " \"description\": \"Position: " << data[i].getPosition().toString(QGeoCoordinate::DegreesWithHemisphere) << " \",\n"
                    " \"availability\": \"" << data[i].getDateTime().toString(Qt::ISODate) << "Z/"
                                           << _latestDateTime.toString(Qt::ISODate) << "Z\",\n"
                    " \"point\":{\n"
                    "  \"color\":{\n"
                    "   \"rgba\":[" << color.red() << "," << color.green() << "," << color.blue() << ",255]\n"
                    " },\n";


        czmlData << " \"outlineColor\":{\n"
                    "  \"rgba\": [" << color.red() << "," << color.green() << "," << color.blue() << ",255]\n"
                    " },\n"
                    " \"outlineWidth\":5,\n"
                    " \"pixelsize\": 200.0,\n"
                    " \"show\": true\n"
                    " },\n";

        czmlData << " \"position\":{\n"
                    "  \"cartographicDegrees\":[" << data[i].getPosition().longitude() << ", "
                                             << data[i].getPosition().latitude() << ", 0]\n"
                    " },\n";


        czmlData << " \"sensorvalue\": " << data[i].getSensorValue() << "\n}";
    }

    //end
    czmlData << "]";

    czmlFile.flush();
    czmlFile.close();

    return 0;
}

int Converter::readCzml(QString filename ,QVector<SensorData> &readSensorData){

    //file opening
    QFile czmlFile;
    QString czmlString;
    czmlFile.setFileName(filename);
    if(!czmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    czmlString = czmlFile.readAll();
    czmlFile.close();


    //json
    QJsonDocument czmlDoc = QJsonDocument::fromJson(czmlString.toUtf8());
    QJsonArray czmlArray = czmlDoc.array(), positionArray;
    QJsonObject czmlObject, positionObject;
    SensorData sensordata;

    for(int i = 1; i < czmlArray.size(); i++){

        czmlObject = czmlArray[i].toObject();
        positionObject = czmlObject["position"].toObject();
        positionArray = positionObject["cartographicDegrees"].toArray();
        QGeoCoordinate parsedPosition;
        parsedPosition.setLatitude(positionArray[1].toDouble());
        parsedPosition.setLongitude(positionArray[0].toDouble());
        sensordata.setHeight(positionArray[2].toDouble());
        sensordata.setPosition(parsedPosition);

        sensordata.setSensorValue(czmlObject["sensorvalue"].toDouble());

        readSensorData.append(sensordata);
    }

    return 0;
}

bool Converter::gpsChecksum(QString &dataline){

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

//find and remove peaks
void Converter::findPeak(QVector<SensorData> &data){

    long long int tempTime;
    long degreeOfFreedom, toDeleteVariable, outlierDataCount;
    double distance, calcSpeed, height, min = 0, max = 0, gCrit, mean, sum, stdev, numerator, sigValue, g;
    bool foundOutlier;
    Grubbs outlierSearchData;
    QVector<Grubbs> outlierSearchDataVector;

    do{
        //calculate Speed between two points and saved it in vektor
        for(int i=0; i< data.count()-1; i++){

            distance = data[i].getPosition().distanceTo(data[i+1].getPosition());
            height = data[i].getHeight();
            tempTime = data[i].getDateTime().secsTo(data[i+1].getDateTime());

            //speed in m/s
            calcSpeed = (qSqrt((qPow(distance, 2)+qPow(height, 2)))/tempTime);

            outlierSearchData.setSpeed(calcSpeed);

            outlierSearchDataVector.append(outlierSearchData);
        }

        outlierDataCount = outlierSearchDataVector.count();

        //locate minimum of vector and calculate sum
        for(int i=0; i < outlierDataCount-1; i++){
            if(min == 0){
                min = outlierSearchDataVector[i].getSpeed();
            } else if(min > outlierSearchDataVector[i].getSpeed()){
                min = outlierSearchDataVector[i].getSpeed();
            }

            sum += outlierSearchDataVector[i].getSpeed();
        }

        //calculate average over all values
        mean = sum / outlierSearchDataVector.count();

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
        double tCrit = alglib::invstudenttdistribution(degreeOfFreedom, sigValue);

        //calculate gCrit -- all points with G over gCrit are outliers
        gCrit = (outlierDataCount-1)*tCrit/qSqrt(outlierDataCount*((outlierDataCount-2)+qPow(tCrit, 2)));

        //calculate G for all values
        for(int i=0; i < outlierSearchDataVector.count()-1; i++){
            g = (mean - outlierSearchDataVector[i].getSpeed()) / stdev;
            outlierSearchDataVector[i].setG(g);

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
        if(toDeleteVariable != -1){
            outlierSearchDataVector.remove(toDeleteVariable);
            data.remove(toDeleteVariable);
            foundOutlier = true;
        }
    } while (foundOutlier);

}

//setter and getter
void Converter::setCompleteSensorData(QVector<SensorData> complete){
    _completeSensorData = complete;
}

void Converter::setIncompleteSensorData(QVector<SensorData> incomplete){
    _incompleteSensorData = incomplete;
}

void Converter::setMaxSensorValue(double value){
    _maxSensorValue = value;
}

void Converter::setMinSensorValue(double value){
    _minSensorValue = value;
}

void Converter::setMaxVehicleSpeed(double speed){
    _maxVehicleSpeed = speed;
}

const QVector<SensorData>& Converter::getCompleteSensorData() {
    return _completeSensorData;
}

const QVector<SensorData>& Converter::getIncompleteSensorData() {
    return _incompleteSensorData;
}

double Converter::getMaxSensorValue() const{
    return _maxSensorValue;
}

double Converter::getMinSensorValue() const{
    return _minSensorValue;
}

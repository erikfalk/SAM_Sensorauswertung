#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QtMath>

#include "grubbs.h"
#include "converter.h"
#include "specialfunctions.h"
#include "converter.h"


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
    long lineId = 0;
    while(!rawData.atEnd()){

       QString line = rawData.readLine();

       if(line.contains("$GPRMC")) {
        lineId++;
        //check for complete dataset
        if(gpsChecksum(line))
            _completeSensorData.append(convertString(lineId, line));
       }
     }

    rawDataFile.flush();
    rawDataFile.close();

    //find and remove peaks

    findPeak(_completeSensorData);


    return 0;
}

//calculates the gps checksum
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

//extract specific Data from a GPS rawdata csv file
SensorData Converter::convertString(long dataId, QString &rawDataString){

    SensorData sensorDataTemp(dataId);
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
    QTime time;
    if((splittedData.at(1)).count() > 6)
         time = QTime::fromString(splittedData.at(1), "hhmmss.zzz");
    else
        time = QTime::fromString(splittedData.at(1), "hhmmss");

    //extract date
    QDate date = QDate::fromString(splittedData.at(9), "ddMMyy");
    date = date.addYears(100);

    sensorDataTemp.setDateTime(QDateTime(date, time));
    //qDebug() << sensorDataTemp.getDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");

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
    }

    //extract latest date and time
    if(sensorDataTemp.getDateTime() > _latestDateTime)
        _latestDateTime = sensorDataTemp.getDateTime();

    return sensorDataTemp;
}

//creates a czml file
int Converter::writeCzml (QDir filePath, const QVector<SensorData>& data){
    QDateTime time;
    QString fileName = time.currentDateTime().toString("ss_ddMMyyyy") + ".czml";

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


        czmlData << ",\n{\n \"id\": \"Monitoring point: " << data[i].getId() << "\",\n"
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

//returns a color, between red and green, corrosponding to the sensorvalue
QColor Converter::mapValueToColor(double sensorValue){

    //calculate hue
    double hue = (_maxSensorValue-sensorValue)/((_maxSensorValue-_minSensorValue)*3);

    //calculate corrosponding color
    QColor color = QColor::fromHsvF(hue, 1.0, 1.0);

    return color.toRgb();
}

void Converter::findMinMaxSensorValue() {

    double sensorValue;
    for(int i = 0; i < _completeSensorData.count(); i++){
        sensorValue = _completeSensorData.at(i).getSensorValue();

        if(sensorValue >= _maxSensorValue)
            _maxSensorValue = sensorValue;

        if(sensorValue <= _minSensorValue)
            _minSensorValue = sensorValue;
    }
}

//read data from czml file and put it into a Vector
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
        QString id = czmlObject["id"].toString();
        sensordata.setId(getIdFromCzmlString(id));

        positionObject = czmlObject["position"].toObject();
        positionArray = positionObject["cartographicDegrees"].toArray();
        QGeoCoordinate parsedPosition;
        parsedPosition.setLatitude(positionArray[1].toDouble());
        parsedPosition.setLongitude(positionArray[0].toDouble());
        sensordata.setPosition(parsedPosition);
        sensordata.setHeight(positionArray[2].toDouble());

        sensordata.setSensorValue(czmlObject["sensorvalue"].toDouble());

        readSensorData.append(sensordata);
    }

    return 0;
}

//find and remove peaks
void Converter::findPeak(QVector<SensorData> &data){

    long int millisecsToPoint;
    long degreeOfFreedom, toDeleteVariable = -1, outlierDataCount;
    double timeToPoint, distance, calcSpeed, heightDif, max = 0, tCrit, gCrit, mean, sum = 0, stdev, numerator = 0, sigValue, g;
    bool foundOutlier;
    Grubbs outlierSearchData;
    QVector<Grubbs> outlierSearchDataVector;

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

long Converter::getIdFromCzmlString(QString idString){
    QRegExp numberFilter("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
    numberFilter.indexIn(idString);
    QStringList idList = numberFilter.capturedTexts();
    if(idList.isEmpty())
        return -1;

    return idList.begin()->toLong();
}

//setter and getter
void Converter::setCompleteSensorData(QVector<SensorData> complete){
    _completeSensorData = complete;
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

QVector<SensorData>& Converter::getCompleteSensorData() {
    return _completeSensorData;
}


double Converter::getMaxSensorValue() const{
    return _maxSensorValue;
}

double Converter::getMinSensorValue() const{
    return _minSensorValue;
}

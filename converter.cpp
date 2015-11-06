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

    SensorData temp;
    QStringList list;

    //split line into tokens and store in list
    list = rawDataString.split(",");

    //claculate lat and lon
    QString deg;
    QString min;

    //build lat
    //extract degrees
    deg.append(list[3].at(0));
    deg.append(list[3].at(1));
    min = list[3].remove(0,2);
    //calculate and set degrees
    temp.position.setLatitude(deg.toDouble() + ((min.toDouble())/60));


    //build lon
    //extract degrees
    deg.clear();
    deg.append(list[5].at(0));
    deg.append(list[5].at(1));
    deg.append(list[5].at(2));
    min = list[5].remove(0,3);
    //calculate degrees
    temp.position.setLongitude(deg.toDouble() + ((min.toDouble())/60));

    //extract time
    QTime time(QString(QString(list[1].at(0)) + QString(list[1].at(1))).toInt(),
               QString(QString(list[1].at(2)) + QString(list[1].at(3))).toInt(),
               QString(QString(list[1].at(4)) + QString(list[1].at(5))).toInt());

    //extract date
    QDate date(2000 + QString(QString(list[9].at(4)) + QString(list[9].at(5))).toInt(),
               QString(QString(list[9].at(2)) + QString(list[9].at(3))).toInt(),
               QString(QString(list[9].at(0)) + QString(list[9].at(1))).toInt());

    temp.dateTime.setTime(time);
    temp.dateTime.setDate(date);

    //extract speed and course over ground
    temp.sog = list[8].toDouble();
    temp.cog = list[9].toDouble();

    //check for and add extra data
    if(list.count() > 13){
        //extract height
         temp.height = list[13].toDouble();

        //extract sensor value
         temp.sensor_value = list[14].toDouble();

        //extract minimum and maximum sensor value
        if(temp.sensor_value > _maxSensorValue)
            _maxSensorValue = temp.sensor_value;

        if(temp.sensor_value < _minSensorValue)
            _minSensorValue = temp.sensor_value;

        //extract latest date and time
        if(temp.dateTime > _latestDateTime)
            _latestDateTime = temp.dateTime;

    }
    return temp;
}


//creates a czml file
int Converter::writeCzml (QString filename, const QVector<SensorData>& data){

    QFile czmlFile(filename);
    

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
        QColor color = mapValueToColor(data[i].sensor_value);
        qDebug() << data[i].dateTime.toString(Qt::ISODate);

        czmlData << ",\n{\n \"id\": \"Dataset " << i << "\",\n"
                    " \"description\": \"Position: " << data[i].position.toString(QGeoCoordinate::DegreesWithHemisphere) << " \",\n"
                    " \"availability\": \"" << data[i].dateTime.toString(Qt::ISODate) << "Z/"
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
                    "  \"cartographicDegrees\":[" << data[i].position.longitude() << ", "
                                             << data[i].position.latitude() << ", 0]\n"
                    " }\n"
                    "}";
    }

    //end
    czmlData << "]";

    czmlFile.flush();
    czmlFile.close();

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
    double distance, calcSpeed;

    for(int i=0; i< data.count()-1; i++){

        distance = data[i].position.distanceTo(data[i+1].position);
        tempTime = data[i].dateTime.secsTo(data[i+1].dateTime);

        //speed in km/h
        calcSpeed = (distance/tempTime)*3.6;

        if(_maxVehicleSpeed <= calcSpeed){
            data.remove(i);
        }
    }
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

const QVector<SensorData>& Converter::getCompleteSensorData(){
    return _completeSensorData;
}

const QVector<SensorData>& Converter::getIncompleteSensorData(){
    return _incompleteSensorData;
}

double Converter::getMaxSensorValue() const{
    return _maxSensorValue;
}

double Converter::getMinSensorValue() const{
    return _minSensorValue;
}

#include "gpschecksum.h"

bool gpsChecksum(QString &dataset){

    QByteArray datasetBytes = dataset.toUtf8();
    QString recieved_checksum;
    int calc_checksum = 0x00;


    for(int i = 0; i < datasetBytes.length(); i++){

            switch(datasetBytes[i]){

                case '$': break;

                case '*': {
                    //extract recieved checksum
                    recieved_checksum.append(datasetBytes.at(i+1));
                    recieved_checksum.append(datasetBytes.at(i+2));
                    i = datasetBytes.length();
                    break;
                }

                default: {
                    calc_checksum ^= datasetBytes[i];
                }
            }
    }

    bool ok;
    if(calc_checksum == recieved_checksum.toInt(&ok,16))
        return true;

    return false;
}

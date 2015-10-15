#include "gpschecksum.h"

bool gpsChecksum(QString &dataline){

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

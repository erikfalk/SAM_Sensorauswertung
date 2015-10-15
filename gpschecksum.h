#ifndef GPSCHECKSUM
#define GPSCHECKSUM

#include <QString>
#include "converter.h"

//returns true if GPS Dataset Checksum is correct
bool gpsChecksum(QString &dataline);

#endif // GPSCHECKSUM


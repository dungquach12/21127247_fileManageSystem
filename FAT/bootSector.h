#pragma once

#include"Utility.h"

// a struct to get info from boot sector
class bootSector {
private:
    uint16_t BytesPerSec;
    uint8_t SecPerClus;
    uint16_t BootSecSize;
    uint8_t NumFatTable;
    uint32_t TotalSector32;
    uint32_t FatTableSize32;
    uint32_t FirstRootCluster; // the first cluster of RDET(FAT32)

    uint32_t FirstRDETSector; // first sector of RDET
    uint32_t FirstDataSector; // first sector of Data

    string FileSysType;

public:
    bootSector();
 
    void getInfo(BYTE arr[]);
    void showInfo();

    uint16_t getBytesPerSec();
    uint8_t getSecPerClus();
    uint16_t getBootSecSize();
    uint32_t getFirstDataSector();
    
    int getInfo(LPCWSTR diskLoc);
};



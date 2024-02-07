#pragma once

#include"Utility.h"

// a struct to get info from boot sector
class bootSector {
private:
    uint16_t BytesPerSec;
    int SecPerClus;
    uint16_t BootSecSize;
    int NumFatTable;
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
    uint16_t getBootSecSize();
    
    int getInfo(LPCWSTR diskLoc);
};



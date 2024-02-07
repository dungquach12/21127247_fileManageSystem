#pragma once

#include"Utility.h"

// a struct to get info from boot sector
class bootSector {
private:
    uint16_t BytesPerSec;
    int SecPerClus;
    uint16_t BootSecSize;
    int NumFatTable;
    uint16_t RootEntryCount; // Number of entry in RDET (FAT32 = 0)
    uint16_t TotalSector16; // total number of sector in Volume (FAT32 = 0)
    uint16_t FatTableSize16; // number of sector in a FAT12, 16 table (FAT32 = 0)
    uint32_t TotalSector32;
    uint32_t FatTableSize32;
    uint32_t FirstRootCluster; // the first cluster of RDET(FAT32)

    uint32_t TotalSecRDET; // total number of sector in RDET
    uint32_t FirstRDETSector; // first sector of RDET
    uint32_t FirstDataSector; // first sector of Data

    string FileSysType;

public:
    bootSector();
 
    void getInfo(BYTE arr[]);
    void showInfo();
    bool isFAT32();

    int showInfo(LPCWSTR diskLoc);
};



#pragma once

#include<iostream>
#include<string>
#include<windows.h>
#include<stdio.h>

using std::cout;
using std::endl;
using std::string;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);

// convert from hex to char
std::string hexToString(BYTE arr[], int startLoc, int size);

// a struct to get info from boot sector
struct bootSector {
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

    std::string FileSysType;

    void getInfo(BYTE arr[]);
    void showInfo();
};



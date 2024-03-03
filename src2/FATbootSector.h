#pragma once
#include<iostream>
#include<windows.h>
#include<stdio.h>
#include<string>
#include<vector>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[]);

// convert from hex to char
string hexToString(BYTE arr[], int startLoc, int size);

string clearExcessSpace(string str);

// find first sector of kth cluster
int firstSectorofCluster(int FirstDataSector, int SecPerClus, int clusOrd);

int isNTFSorFAT32(BYTE bootSector[]);

class FATbootSector {
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
    LPCWSTR drive;
    FATbootSector();

    void getInfo(BYTE arr[]);
    void showInfo();

    uint16_t getBytesPerSec();
    uint8_t getSecPerClus();
    uint16_t getBootSecSize();
    uint32_t getFirstRootClus();
    uint32_t getFirstDataSector();

    int getInfo(LPCWSTR diskLoc);
};



// To traverse the FAT table and get the list of cluster of an entry
vector<uint32_t> getListClusters(int firstCluster, FATbootSector disk);
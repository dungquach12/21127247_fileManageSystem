#pragma once

#include<iostream>
#include<windows.h>
#include<stdio.h>
#include<string>
#include<vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[]);

// convert from hex to char
string hexToString(BYTE arr[], int startLoc, int size);

// find first sector of kth cluster
int firstSectorofCluster(int FirstDataSector, int SecPerClus, int clusOrd);

// To traverse the FAT table and get the list of cluster of an entry
vector<uint32_t> getListClusters(uint32_t firstCluster, int BootSecSize, int BytesPerSec);
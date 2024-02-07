#pragma once

#include<iostream>
#include<windows.h>
#include<stdio.h>
#include<string>

using std::cout;
using std::endl;
using std::string;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);

// convert from hex to char
string hexToString(BYTE arr[], int startLoc, int size);

// find first sector of kth cluster
int firstSectorofCluster(int FirstDataSector, int SecPerClus, int clusOrd);


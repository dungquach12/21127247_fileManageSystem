#pragma once

#include"FATbootSector.h"

class File {
public:
    string fileName;
    string fileExtension;
    uint8_t attribute;
    uint16_t firstCluster;
    uint32_t fileSize;

    File();
};

int getFiles(int firstCluster, FATbootSector disk, vector<File>& list);
string convertAttrNumToAttrString(uint8_t attrNum);
int interactFile(File theFile, FATbootSector disk);
int Directory(FATbootSector disk, int cluster);
#pragma once

#include"bootSector.h"

class File {
public:
    string fileName;
    string fileExtension;
    uint8_t attribute;
    uint16_t firstCluster;
    uint32_t fileSize;

    File();
};

int getFiles(int firstCluster, bootSector disk, vector<File>& list);
string convertAttrNumToAttrString(uint8_t attrNum);
int interactFile(File theFile, bootSector disk);
int Directory(bootSector disk, int cluster);
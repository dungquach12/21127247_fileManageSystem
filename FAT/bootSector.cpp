#include"bootSector.h"

bootSector::bootSector() {
    this->BytesPerSec = 0;
    this->SecPerClus = 0;
    this->BootSecSize = 0;
    this->NumFatTable = 0;
    this->TotalSector32 = 0;
    this->FatTableSize32 = 0;
    this->FirstRootCluster = 0;
    this->FirstRDETSector = 0;
    this->FirstDataSector = 0;

    this->FileSysType = ' ';
}

void bootSector::getInfo(BYTE arr[]) {
    memcpy(&this->BytesPerSec, arr + 0xB, 2);
    this->SecPerClus = arr[0xD];
    memcpy(&this->BootSecSize, arr + 0xE, 2);
    this->NumFatTable = arr[0x10];
    memcpy(&this->TotalSector32, arr + 0x20, 4);

    // Get File System Type
    this->FileSysType = hexToString(arr, 0x52, 8);

    memcpy(&this->FatTableSize32, arr + 0x24, 4);
    memcpy(&this->FirstRootCluster, arr + 0x2C, 4);
    this->FirstDataSector = this->BootSecSize + this->NumFatTable * this->FatTableSize32; // Get first sector of Data region
    this->FirstRDETSector = firstSectorofCluster(this->FirstDataSector, this->SecPerClus, this->FirstRootCluster);
}


void bootSector::showInfo() {
    cout << "Loai Fat:                              " << this->FileSysType << endl;
    cout << "-------------------------------------------" << endl;
    cout << "So byte tren moi sector(byte):         " << this->BytesPerSec << endl;
    cout << "So sector tren cluster:                " << int(this->SecPerClus) << endl;
    cout << "So sector thuoc vung bootsector:       " << this->BootSecSize << endl;
    cout << "So bang Fat:                           " << int(this->NumFatTable) << endl;
    cout << "Kich thuoc volume(sector):             " << this->TotalSector32 << endl;
    cout << "Kich thuoc bang FAT(sector/FAT):       " << this->FatTableSize32 << endl;
    cout << "Cluster bat dau cua RDET:              " << this->FirstRootCluster << endl;
    cout << "Sector dau tien RDET:                  " << this->FirstRDETSector << endl;
    cout << "Sector dau tien vung data:             " << this->FirstDataSector << endl;
}

uint16_t bootSector::getBytesPerSec() {
    return this->BytesPerSec;
}

uint8_t bootSector::getSecPerClus() {
    return this->SecPerClus;
}

uint16_t bootSector::getBootSecSize() {
    return this->BootSecSize;
}

uint32_t bootSector::getFirstDataSector() {
    return this->FirstDataSector;
}

int bootSector::getInfo(LPCWSTR diskLoc) {
    BYTE sector[512];
    if (!ReadSector(diskLoc, 0, sector)) {
        this->getInfo(sector);
        return 0;
    }
    return 1;
}
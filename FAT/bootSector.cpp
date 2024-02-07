#include"bootSector.h"

bootSector::bootSector() {
    this->BytesPerSec = 0;
    this->SecPerClus = 0;
    this->BootSecSize = 0;
    this->NumFatTable = 0;
    this->RootEntryCount = 0;
    this->TotalSector16 = 0;
    this->FatTableSize16 = 0;
    this->TotalSector32 = 0;
    this->FatTableSize32 = 0;
    this->FirstRootCluster = 0;

    this->TotalSecRDET = 0;
    this->FirstRDETSector = 0;
    this->FirstDataSector = 0;

    this->FileSysType = ' ';
}

void bootSector::getInfo(BYTE arr[]) {
    memcpy(&this->BytesPerSec, arr + 0xB, 2);
    this->SecPerClus = arr[0xD];
    memcpy(&this->BootSecSize, arr + 0xE, 2);
    this->NumFatTable = arr[0x10];
    memcpy(&this->RootEntryCount, arr + 0x11, 2);
    memcpy(&this->TotalSector16, arr + 0x13, 2);
    memcpy(&this->FatTableSize16, arr + 0x16, 2);
    memcpy(&this->TotalSector32, arr + 0x20, 4);

    // Get File System Type
    if (this->TotalSector16 == 0 && TotalSector32 != 0)
        this->FileSysType = hexToString(arr, 0x52, 8);
    else
        this->FileSysType = hexToString(arr, 0x36, 8);

    // if this is FAT32 get 2 more info
    if (this->TotalSector16 == 0 && TotalSector32 != 0) {
        memcpy(&this->FatTableSize32, arr + 0x24, 4);
        memcpy(&this->FirstRootCluster, arr + 0x2C, 4);
        this->FirstDataSector = this->BootSecSize + this->NumFatTable * this->FatTableSize32; // Get first sector of Data region
        this->FirstRDETSector = firstSectorofCluster(this->FirstDataSector, this->SecPerClus, this->FirstRootCluster);
    }
    else {
        this->FirstRDETSector = this->BootSecSize + this->NumFatTable * this->FatTableSize16; // Get first sector of RDET (FAT12, 16)
        this->TotalSecRDET = (this->RootEntryCount * 32) / this->BytesPerSec; // Total sector of RDET (FAT12, 16)
        this->FirstDataSector = this->FirstRDETSector + this->TotalSecRDET; // Get first sector of Data (FAT12, 16)
    }
}

bool bootSector::isFAT32() {
    if (this->TotalSector16 == 0 && TotalSector32 != 0) {
        return true;
    }
    return false;
}

void bootSector::showInfo() {
    cout << "Loai Fat:                              " << this->FileSysType << endl;
    cout << "-------------------------------------------" << endl;
    cout << "So byte tren moi sector(byte):         " << this->BytesPerSec << endl;
    cout << "So sector tren cluster:                " << this->SecPerClus << endl;
    cout << "So sector thuoc vung bootsector:       " << this->BootSecSize << endl;
    cout << "So bang Fat:                           " << this->NumFatTable << endl;

    if (this->isFAT32()) {
        cout << "Kich thuoc volume(sector):             " << this->TotalSector32 << endl;
        cout << "Kich thuoc bang FAT(sector/FAT):       " << this->FatTableSize32 << endl;
        cout << "Cluster bat dau cua RDET:              " << this->FirstRootCluster << endl;
    }
    else {
        cout << "Kich thuoc volume(sector):             " << this->TotalSector16 << endl;
        cout << "Kich thuoc bang FAT(sector/FAT):       " << this->FatTableSize16 << endl;
        cout << "So entry tren bang RDET:               " << this->RootEntryCount << endl;
        cout << "Kich thuoc bang RDET(sector):          " << this->TotalSecRDET << endl;
    }
    cout << "Sector dau tien RDET:                  " << this->FirstRDETSector << endl;
    cout << "Sector dau tien vung data:             " << this->FirstDataSector << endl;
}

int bootSector::showInfo(LPCWSTR diskLoc) {
    BYTE sector[512];
    if (!ReadSector(diskLoc, 0, sector)) {
        this->getInfo(sector);
        this->showInfo();
        return 0;
    }
    return 1;
}
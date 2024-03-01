#include"bootSector.h"

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint * 512, NULL, FILE_BEGIN);

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }

    return 0;
}

string hexToString(BYTE arr[], int startLoc, int size) {
    std::string res;
    for (int i = startLoc; i < startLoc + size; i++)
        res += arr[i];
    res += '\0';
    return res;
}

int firstSectorofCluster(int FirstDataSector, int SecPerClus, int clusOrd) {
    return FirstDataSector + (clusOrd - 2) * SecPerClus;
}

vector<uint32_t> getListClusters(uint32_t firstCluster, int BootSecSize, int BytesPerSec)
{
    vector<uint32_t> listClusters;
    BYTE sectorFat[512];
    uint32_t nextCluster = firstCluster;

    do {
        int thisFATSecNum = BootSecSize + nextCluster / ((BytesPerSec / 4) - 1);

        int thisFATEntOffset = nextCluster * 4;
        while (thisFATEntOffset >= 512) {
            thisFATEntOffset -= 512;
        }
        listClusters.push_back(nextCluster);
        ReadSector(L"\\\\.\\E:", thisFATSecNum, sectorFat);

        memcpy(&nextCluster, sectorFat + thisFATEntOffset, 4);
    } while (nextCluster != 0x0FFFFFFF && nextCluster != 0x0FFFFFF8);

    return listClusters;
}

FATbootSector::FATbootSector() {
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

void FATbootSector::getInfo(BYTE arr[]) {
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


void FATbootSector::showInfo() {
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

uint16_t FATbootSector::getBytesPerSec() {
    return this->BytesPerSec;
}

uint8_t FATbootSector::getSecPerClus() {
    return this->SecPerClus;
}

uint16_t FATbootSector::getBootSecSize() {
    return this->BootSecSize;
}

uint32_t FATbootSector::getFirstRootClus() {
    return this->FirstRootCluster;
}

uint32_t FATbootSector::getFirstDataSector() {
    return this->FirstDataSector;
}

int FATbootSector::getInfo(LPCWSTR diskLoc) {
    BYTE sector[512];
    if (!ReadSector(diskLoc, 0, sector)) {
        this->getInfo(sector);
        return 0;
    }
    return 1;
}
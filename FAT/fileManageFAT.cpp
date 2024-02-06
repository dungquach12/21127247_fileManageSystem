#include"fileManageFAT.h"

std::string hexToString(BYTE arr[], int startLoc, int size) {
    std::string res;
    for (int i = startLoc; i < startLoc + size; i++)
        res += arr[i];
    return res;
}

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
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

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Success!\n");
    }
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

    // if this is FAT32 get 2 more info
    if (this->TotalSector16 == 0 && TotalSector32 != 0) {
        memcpy(&this->FatTableSize32, arr + 0x24, 4);
        memcpy(&this->FirstRootCluster, arr + 0x2C, 4);
    }

    this->TotalSecRDET = (this->RootEntryCount * 32) / this->BytesPerSec;

    // Get first sector of RDET
    if (this->TotalSector16 == 0 && TotalSector32 != 0)
        this->FirstRDETSector = this->BootSecSize + this->NumFatTable * this->FatTableSize32;
    else
        this->FirstRDETSector = this->BootSecSize + this->NumFatTable * this->FatTableSize16;

    // Get first sector of Data
    this->FirstDataSector = this->FirstRDETSector + this->TotalSecRDET;

    // Get File System Type
    if (this->TotalSector16 == 0 && TotalSector32 != 0)
        this->FileSysType = hexToString(arr, 0x52, 8);
    else
        this->FileSysType = hexToString(arr, 0x36, 8);
}

void bootSector::showInfo() {
    cout << "Loai Fat:                              " << this->FileSysType << endl;
    cout << "-------------------------------------------" << endl;
    cout << "So byte tren moi sector(byte):         " << this->BytesPerSec << endl;
    cout << "So sector tren cluster:                " << this->SecPerClus << endl;
    cout << "So sector thuoc vung bootsector:       " << this->BootSecSize << endl;
    cout << "So bang Fat:                           " << this->NumFatTable << endl;
    cout << "So entry tren bang RDET:               " << this->RootEntryCount << endl;

    if (this->TotalSector16 == 0 && TotalSector32 != 0) {
        cout << "Kich thuoc volume(sector):             " << this->TotalSector32 << endl;
        cout << "Kich thuoc bang FAT(sector/FAT):       " << this->FatTableSize32 << endl;
        cout << "Cluster bat dau cua RDET:              " << this->FirstRootCluster << endl;
    }
    else {
        cout << "Kich thuoc volume(sector):             " << this->TotalSector16 << endl;
        cout << "Kich thuoc bang FAT(sector/FAT):       " << this->FatTableSize16 << endl;
    }
    cout << endl;
    cout << "-------------------------------------------" << endl;
    cout << "Kich thuoc bang RDET(sector):          " << this->TotalSecRDET << endl;
    cout << "Sector dau tien RDET:                  " << this->FirstRDETSector << endl;
    cout << "Sector dau tien vung data:             " << this->FirstDataSector << endl;
}
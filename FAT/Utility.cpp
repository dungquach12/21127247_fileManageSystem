#include"Utility.h"

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
    else
    {
        printf("Success!\n");
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

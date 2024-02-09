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

        int thisFATEntOffset = firstCluster * 4;
        while (thisFATEntOffset >= 512) {
            thisFATEntOffset -= 512;
        }

        listClusters.push_back(nextCluster);
        ReadSector(L"\\\\.\\E:", thisFATSecNum, sectorFat);

        memcpy(&nextCluster, sectorFat + thisFATEntOffset, 4);
    } while (nextCluster != 0x0FFFFFFF && nextCluster != 0x0FFFFFF8);

    return listClusters;
}

#pragma once

class NTFSbootSector {
private:
    uint16_t BytesPerSec;
    uint8_t SecPerClus;

    uint16_t SecPerTrack;
    uint16_t HeadNum;
    uint64_t TotalSector;
    uint64_t FirstMFTClus;
    uint8_t BytesPerEntry;
public:
    int getInfo();
    int showInfo();

    int getInfo(LPCWSTR diskLoc);
};

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

File::File() {
    this->fileName = ' ';
    this->fileExtension = ' ';
    this->attribute = 0;
    this->firstCluster = 0;
    this->fileSize = 0;
}

int main(int argc, char** argv)
{
    bootSector disk1;
    disk1.getInfo(L"\\\\.\\E:");
    //disk1.showInfo();

    cout << "-------------------------" << endl;
    
    vector<uint32_t> listcluster;
    listcluster = getListClusters(2, disk1.getBootSecSize(), disk1.getBytesPerSec());

    int fileCount = 0;
    vector<File> fileList;

    for (uint32_t i : listcluster) {
        int sectorNum = firstSectorofCluster(disk1.getFirstDataSector(), disk1.getSecPerClus(), i);

        for (int j = sectorNum; j < sectorNum + 32; j++) {
            BYTE sector[512];
            ReadSector(L"\\\\.\\E:", j, sector);
            for (int k = 0; k < disk1.getBytesPerSec(); k += 32) {
                if (j == sectorNum && k == 0) {
                    k += 64;
                }
                uint8_t entryStatus = sector[k];
                if (entryStatus == 0x2E) {
                    // do something
                    continue;
                }
                if (entryStatus == 0x00 || entryStatus == 0xE5) { // empty entry or deleted entry
                    continue;
                }
                fileCount++;
                File tmp;

                uint8_t attribute = sector[k + 0xB];
                // Primary Entry
                if (attribute == 0x0F) {
                    // count the number of sub-entry of the file
                    int subEntryCount = 1;
                    while (attribute == 0x0F) {
                        attribute = sector[k + 0xB + 32 * subEntryCount];
                        subEntryCount++;
                    }

                    // get the long name of the file
                    for (int i = 0; i < subEntryCount; i++) {
                        // happy lunar new year
                    }
                }
                else {
                    tmp.fileName = hexToString(sector, k, 8);
                    tmp.fileExtension = hexToString(sector, k + 0x8, 3);
                    tmp.attribute = attribute;
                    memcpy(&tmp.firstCluster, sector + k + 0x1A, 2);
                    memcpy(&tmp.firstCluster, sector + k + 0x1C, 4);
                    fileList.push_back(tmp);
                }
            }
        }
    }

    for (File file: fileList) {
        cout << file.fileName << file.fileExtension << endl;
    }

    return 0;
}
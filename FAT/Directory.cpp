#include"Directory.h"

File::File() {
    this->fileName = "";
    this->fileExtension = "";
    this->attribute = 0;
    this->firstCluster = 0;
    this->fileSize = 0;
}

string clearExcessSpace(string str) {
    int strLength = str.length();
    for (int i = strLength; i >= 0; i--) {
        if ((str[i] == 0x20) && i == 0) {
            str.erase(i, 1);
            strLength--;
            break;
        }
        if (str[i] == 0x20 && str[i - 1] == 0x20) {
            str.erase(i, 1);
        }
        if (str[i] <= 0) {
            str.erase(i, 1);
            strLength--;
        }
    }
    str += '\0';
    str.resize(strLength + 1);
    str.shrink_to_fit();
    return str;
}

int getFiles(int firstCluster, bootSector disk, vector<File>& list) {
    vector<uint32_t> listcluster;
    listcluster = getListClusters(firstCluster, disk);

    int fileCount = 0;
    vector<File> fileList;

    for (uint32_t i : listcluster) {
        int sectorNum = firstSectorofCluster(disk.getFirstDataSector(), disk.getSecPerClus(), i);

        string reserveName;
        bool reserveState = FALSE;
        for (int j = sectorNum; j < sectorNum + disk.getSecPerClus(); j++) {
            BYTE sector[512];
            ReadSector(disk.drive, j, sector);

            for (int k = 0; k < disk.getBytesPerSec(); k += 32) {
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
                // Sub Entry
                if (attribute == 0x0F) {
                    // count the number of sub-entry of the file
                    int subEntryCount = 1;
                    while (attribute = sector[k + 0xB + 32 * subEntryCount]) {
                        if (attribute != 0x0F) {
                            break;
                        }
                        subEntryCount++;
                        // if the file's entries span more than 1 entry, save the name
                        if ((k + 0xB + 32 * (subEntryCount)) >= 512) {
                            for (int i = subEntryCount - 1; i >= 0; i--) {
                                reserveName += hexToString(sector, k + 0x1 + 32 * i, 10)
                                    + hexToString(sector, k + 0xE + 32 * i, 12)
                                    + hexToString(sector, k + 0x1C + 32 * i, 4);
                            }
                            reserveState = TRUE;
                            break;
                        }
                    }

                    // break the loop to go to the next sector if file's entries span more than 1 sector
                    if ((k + 0xB + 32 * (subEntryCount)) >= 512) {
                        break;
                    }

                    // get the long name of the file
                    if (reserveState == TRUE) {
                        tmp.fileName = reserveName;
                        reserveState = FALSE;
                    }
                    for (int i = subEntryCount - 1; i >= 0; i--) {
                        tmp.fileName += hexToString(sector, k + 0x1 + 32 * i, 10)
                            + hexToString(sector, k + 0xE + 32 * i, 12)
                            + hexToString(sector, k + 0x1C + 32 * i, 4);
                    }

                    // get other info of the file
                    tmp.attribute = sector[k + 0xB + 32 * subEntryCount];
                    memcpy(&tmp.firstCluster, sector + k + 0x1A + 32 * subEntryCount, 2);
                    memcpy(&tmp.fileSize, sector + k + 0x1C + 32 * subEntryCount, 4);
                    tmp.fileName = clearExcessSpace(tmp.fileName);
                    fileList.push_back(tmp);

                    k += (32 * subEntryCount);
                }
                else {
                    if (reserveState == TRUE) {
                        tmp.fileName = reserveName;
                        reserveState = FALSE;
                    }
                    else {
                        tmp.fileName = hexToString(sector, k, 8);
                        tmp.fileExtension = hexToString(sector, k + 0x8, 3);
                    }
                    tmp.attribute = attribute;
                    memcpy(&tmp.firstCluster, sector + k + 0x1A, 2);
                    memcpy(&tmp.fileSize, sector + k + 0x1C, 4);
                    tmp.fileName = clearExcessSpace(tmp.fileName);
                    tmp.fileExtension = clearExcessSpace(tmp.fileExtension);
                    fileList.push_back(tmp);
                }
            }
        }
    }
    list = fileList;
    return 0;
}

string convertAttrNumToAttrString(uint8_t attrNum) {
    string attrString;
    string attrList[] = { "Read Only", "Hidden", "System", "Volume Label", "Subdirectory", "Archive" };
    for (int i = 0; i < 6; i++) {
        if (attrNum & 1) {
            attrString += attrList[i] + ", ";
        }
        attrNum = attrNum >> 1;
    }
    return attrString;
}

int interactFile(File theFile, bootSector disk) {
    if (theFile.fileExtension.find("TXT") != string::npos || theFile.fileName.find("txt") != string::npos) {
        system("cls");
        vector<uint32_t> listcluster;
        listcluster = getListClusters(theFile.firstCluster, disk);
        int size = theFile.fileSize;
        for (uint32_t i : listcluster) {
            int sectorNum = firstSectorofCluster(disk.getFirstDataSector(), disk.getSecPerClus(), i);
            for (int j = sectorNum; j < sectorNum + disk.getSecPerClus(); j++) {
                BYTE sector[512];
                ReadSector(disk.drive, j, sector);
                if (size > 512) {
                    cout << hexToString(sector, 0, 511);
                    size -= 512;
                }
                else {
                    cout << hexToString(sector, 0, size);
                    break;
                }
                
            }
        }
        cout << endl;
    }
    else {
        cout << "Not directory or txt file" << endl;
    }
    return 0;
}

int Directory(bootSector disk, int cluster) {
    vector<File> list;
    getFiles(cluster, disk, list);

    int option = 0;
    do {
        system("cls");
        for (int i = 0; i < list.size(); i++) {
            cout << "Stt:           " << i + 1 << endl
                << "Name:          " << list[i].fileName << list[i].fileExtension << endl
                << "Attributes:    " << convertAttrNumToAttrString(list[i].attribute) << endl
                << "Size:          " << list[i].fileSize << " (bytes)" << endl
                << "First cluster: " << list[i].firstCluster << endl
                << "----------------------------------------------" << endl;
        }

        cout << "Choose a file/folder (or exit: -1)" << endl;
        cin >> option;
        if (option == -1) {
            break;
        }
        if (option > list.size()) {
            continue;
        }
        if ((list[option - 1].attribute >> 4) & 1) {
            Directory(disk, list[option - 1].firstCluster);
        }
        else {
            interactFile(list[option - 1], disk);
            system("pause");
        }
    } while (true);

    return 0;
}
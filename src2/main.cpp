#include"fat32Dir.h"

int main() {
    BYTE sector[512];
    ReadSector(L"\\\\.\\F:", 0, sector);


    if (isNTFSorFAT32(sector) == 0) { // NTFS
        cout << "NTFS";
    }
    else if (isNTFSorFAT32(sector) == 1) { // FAT32 
        int option = 0;
        FATbootSector disk1;
        if (!disk1.getInfo(L"\\\\.\\F:")) {


            do {
                system("cls");
                cout << "+---------------------------------+\n"
                    << "|1. Volume's infomation           |\n"
                    << "|2. Root directory                |\n"
                    << "|-1. Exit                         |\n"
                    << "+---------------------------------+\n";
                cin >> option;
                if (option == 1) {
                    system("cls");
                    disk1.showInfo();
                    system("pause");

                }
                else if (option == 2) {
                    system("cls");
                    Directory(disk1, disk1.getFirstRootClus());
                }
            } while (option != -1);
            return 0;
        }
    }
    return 0;
}
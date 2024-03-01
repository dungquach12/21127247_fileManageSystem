#include"Directory.h"

int main(int argc, char** argv)
{
    int option = 0;
    FATbootSector disk1;
    if (!disk1.getInfo(L"\\\\.\\E:")) {


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
    cout << "Disk not exist!" << endl;
    return 1;
}

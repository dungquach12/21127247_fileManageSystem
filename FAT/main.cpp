#include"Directory.h"

int main(int argc, char** argv)
{
    char diskName;
    int option = 0;
    bootSector disk1;   
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
    cout << "Disk not exist!" << endl;
    return 1;
}

//string clearExcessSpace(string str) {
//	int strLength = str.length();
//	for (int i = strLength; i >= 0; i--) {
//		if (str[i] == 0x20 && str[i - 1] == 0x20) {
//			str.erase(i, 1);
//			strLength--;
//		}
//	}
//	str.resize(strLength);
//	str.shrink_to_fit();
//	return str;
//}
//
//int main() {
//	string str = "Test  string to   to  eo              ";
//	cout << clearExcessSpace(str);
//}
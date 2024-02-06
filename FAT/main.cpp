#include"fileManageFAT.h"

int main(int argc, char** argv)
{

    BYTE sector[512];
    ReadSector(L"\\\\.\\E:", 0, sector);

    bootSector disk1;
    disk1.getInfo(sector);
    disk1.showInfo();
}
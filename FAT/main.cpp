#include"bootSector.h"

int main(int argc, char** argv)
{
    bootSector disk1;
    disk1.showInfo(L"\\\\.\\E:");
    return 0;
}
#include"bootSector.h"
#include"FAT.h"

int main(int argc, char** argv)
{
    bootSector disk1;
    disk1.getInfo(L"\\\\.\\e:");
    //disk1.showInfo();

    cout << "-------------------------" << endl;
    
    vector<uint32_t> listcluster;
    listcluster = getListClusters(2, disk1);

    for (int i = 0; i < listcluster.size(); i++) {
        cout << listcluster[i] << endl;
    }

    return 0;
}
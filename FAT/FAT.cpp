#include"FAT.h"

vector<uint32_t> getListClusters(uint32_t firstCluster, bootSector info)
{
	vector<uint32_t> listClusters;
	BYTE sectorFat[512];
	uint32_t nextCluster = firstCluster;

	do {
		int thisFATSecNum = info.getBootSecSize() + nextCluster / ((info.getBytesPerSec() / 4) - 1);

		int thisFATEntOffset = firstCluster * 4;
		cout << thisFATSecNum << endl;
		while (thisFATEntOffset >= 512) {
			thisFATEntOffset -= 512;
		}

		listClusters.push_back(nextCluster);
		ReadSector(L"\\\\.\\E:", thisFATSecNum, sectorFat);

		memcpy(&nextCluster, sectorFat + thisFATEntOffset, 4);
	} while (nextCluster != 0x0FFFFFFF && nextCluster != 0x0FFFFFF8);

	return listClusters;
}

/*
* To traverse the FAT table and get the list of cluster of an entry
*/

#pragma once
#include"bootSector.h"
#include<vector>

using std::vector;

vector<uint32_t> getListClusters(uint32_t firstCluster, bootSector info);
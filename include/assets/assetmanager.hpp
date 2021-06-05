#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class Asset {

};

class AssetManager {
    public:
    map<string, Asset> loadedAssets;

    AssetManager() = default;
    void LoadAllAssetsInDirectory(string directory);
    void LoadAsset(string path, string name);
    void GetAsset(string name);
};
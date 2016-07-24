#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
class AssetLoader
{
public:
    static const char* GetAsset(const char* assetPath, unsigned int& assetSize);
    ~AssetLoader() = default;
private:
    static std::unordered_map < std::string, std::shared_ptr<std::vector<char>>> mAssetMap;
};


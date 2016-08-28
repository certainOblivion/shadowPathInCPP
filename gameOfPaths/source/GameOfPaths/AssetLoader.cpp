#include "pch.h"
#include "AssetLoader.h"
#include <fstream>
#include <string>
#include <streambuf>
#include <memory>
#include <functional>
#include "Library\AssertCheck.h"

using namespace std;

std::unordered_map < std::string, std::shared_ptr<std::vector<char>>> AssetLoader::mAssetMap;

const char* AssetLoader::GetAsset(const char* assetPath, unsigned int& assetSize)
{
    throw_assert(assetPath != nullptr, "assetPath is null");
    string path(assetPath);
    shared_ptr<vector<char>> data;
    auto iterator = mAssetMap.find(path);
    if (iterator != mAssetMap.end())
    {
        data = iterator->second;
        assetSize = data->size();
    }
    else
    {
        data = make_shared<vector<char>>();
        std::ifstream file(assetPath, std::ios::binary | ios::ate);
        if (file.bad())
        {
            throw std::exception("Could not open file");
        }

        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());

        if (size > 0)
        {
            data->resize(size);
            file.seekg(0, std::ios::beg);
            file.read(&data->front(), size);
        }

        file.close();

        assetSize = size;
        mAssetMap[path] = data;
    }
    return &((*data)[0]);
}

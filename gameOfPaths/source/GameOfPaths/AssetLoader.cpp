#include "pch.h"
#include "AssetLoader.h"
#include <fstream>
#include <string>
#include <streambuf>
#include <memory>
#include <functional>
#include "Library\AssertCheck.h"
#include <sys/stat.h>

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
        struct stat buffer;
        int fileStat = stat(assetPath, &buffer);
        throw_assert(fileStat == 0, "%s file does not exist", assetPath);

        data = make_shared<vector<char>>();
        std::ifstream file(assetPath, std::ios::binary | ios::ate);

        throw_assert(!file.bad(), "Read/writing error on file read operation. File: %s" , assetPath);

        size_t size = buffer.st_size;

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

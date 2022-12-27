#include "../include/AssetManager.h"

#include <unordered_map>
#include <filesystem>
#include <vector>

using namespace std;

static unordered_map<string, Texture2D> textureBuffer;

void LoadAssets()
{
    string path = "assets";
    vector<string> fileNames;
    for(auto& entry : filesystem::directory_iterator(path))
    {
        fileNames.push_back(entry.path().string());
    }

    for(auto& fileName : fileNames)
    {
        const char* texturePath = fileName.c_str();
        string textureName = fileName.substr(7);
        Texture2D texture = LoadTexture(texturePath);
        textureBuffer.insert({textureName, texture});
    }
}

void UnloadAssets()
{
    for(auto& texture : textureBuffer)
    {
        UnloadTexture(texture.second);
    }

    textureBuffer.clear();
}

Texture2D* GetTexture(const string& textureName)
{
    Texture2D* texture = nullptr;

    auto iteratorMap = textureBuffer.find(textureName);

    if(iteratorMap != textureBuffer.end())
    {
        auto iteratorPair = *iteratorMap;
        texture = &iteratorPair.second;
    }

    return texture;
}
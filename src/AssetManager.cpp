#include "../include/AssetManager.h"

#include <unordered_map>
#include <filesystem>
#include <vector>

using namespace std;

static unordered_map<string, Texture2D> textureBuffer;
static unordered_map<string, Texture2D> uiBuffer;

static vector<string> getFileNames(string path);
static unordered_map<string, Texture2D> fillBuffer(vector<string> fileNames, size_t pos);

void LoadAssets()
{
    // string assetPath = "assets";
    // vector<string> assetFileNames;
    // for(auto& entry : filesystem::directory_iterator(assetPath))
    // {
    //     assetFileNames.push_back(entry.path().string());
    // }

    // for(auto& fileName : assetFileNames)
    // {
    //     const char* texturePath = fileName.c_str();
    //     string textureName = fileName.substr(7);
    //     Texture2D texture = LoadTexture(texturePath);
    //     textureBuffer.insert({textureName, texture});
    // }

    // string uiPath = "assets/ui";
    // vector<string> uiFileNames;
    vector<string> assetFileNames = getFileNames("assets");
    textureBuffer = fillBuffer(assetFileNames, 7);

    vector<string> uiFileNames = getFileNames("assets/ui");
    uiBuffer = fillBuffer(uiFileNames, 10);
}

void UnloadAssets()
{
    for(auto& texture : textureBuffer)
    {
        UnloadTexture(texture.second);
    }

    textureBuffer.clear();
}

// Returning pointer of vector might not be a good idea!
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

// Returning pointer of vector might not be a good idea!
Texture2D* GetTextureUI(const string& uiName)
{
    Texture2D* texture = nullptr;

    auto iteratorMap = uiBuffer.find(uiName);

    if(iteratorMap != uiBuffer.end())
    {
        auto iteratorPair = *iteratorMap;
        texture = &iteratorPair.second;
    }

    return texture;
}

static vector<string> getFileNames(string path)
{
    vector<string> fileNames;

    for(auto& entry : filesystem::directory_iterator(path))
    {
        fileNames.push_back(entry.path().string());
    }

    return fileNames;
}

static unordered_map<string, Texture2D> fillBuffer(vector<string> fileNames, size_t pos)
{
    unordered_map<string, Texture2D> buffer;

    for(auto fileName : fileNames)
    {
        const char* texturePath = fileName.c_str();
        string textureName = fileName.substr(pos);
        Texture2D texture = LoadTexture(texturePath);
        buffer.insert({textureName, texture});
    }

    return buffer;
}

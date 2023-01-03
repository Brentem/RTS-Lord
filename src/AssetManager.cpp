#include "../include/AssetManager.h"

#include <filesystem>
#include <vector>
#include <stdexcept>

using namespace std;

AssetManager* AssetManager::assetManagerInstance_{nullptr};
mutex AssetManager::mutex_;

AssetManager::AssetManager()
{
    string path = "assets/ui";
    vector<string> fileNames;
    for(auto& entry : filesystem::directory_iterator(path))
    {
        fileNames.push_back(entry.path().string());
    }

    for(auto& fileName : fileNames)
    {
        const char* texturePath = fileName.c_str();
        string textureName = fileName.substr(10);
        Texture2D texture = LoadTexture(texturePath);
        uiTextureBuffer.insert({textureName, texture});
    }

    path = "assets";
    fileNames.clear();
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

AssetManager::~AssetManager()
{
    for(auto& texture : uiTextureBuffer)
    {
        UnloadTexture(texture.second);
    }

    uiTextureBuffer.clear();
}

AssetManager* AssetManager::GetInstance()
{
    lock_guard<mutex> lock(mutex_);

    if(assetManagerInstance_ == nullptr)
    {
        assetManagerInstance_ = new AssetManager();
    }

    return assetManagerInstance_;
}

void AssetManager::DeleteInstance()
{
    lock_guard<mutex> lock(mutex_);

    if(assetManagerInstance_ != nullptr)
    {
        delete assetManagerInstance_;
        assetManagerInstance_ = nullptr;
    }
}

Texture2D& AssetManager::GetTexture(const string& textureName)
{
    auto iteratorMap = textureBuffer.find(textureName);

    if(iteratorMap == textureBuffer.end())
    {
        throw invalid_argument("Given uiTextureName doesn't give a valid texture!");
    }

    auto& iteratorPair = *iteratorMap;
    
    return iteratorPair.second;
}

Texture2D& AssetManager::GetTextureUI(const string& uiTextureName)
{
    auto iteratorMap = uiTextureBuffer.find(uiTextureName);

    if(iteratorMap == uiTextureBuffer.end())
    {
        throw invalid_argument("Given uiTextureName doesn't give a valid texture!");
    }

    auto& iteratorPair = *iteratorMap;
    
    return iteratorPair.second;
}
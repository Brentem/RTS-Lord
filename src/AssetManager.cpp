#include "../include/AssetManager.h"

#include <filesystem>
#include <vector>
#include <stdexcept>

using namespace std;

AssetManager* AssetManager::assetManagerInstance_{nullptr};
mutex AssetManager::mutex_;

AssetManager::AssetManager()
{
    fillBuffer("assets", 7);
    fillBuffer("assets/ui", 10);
}

AssetManager::~AssetManager()
{
    for(auto& texture : textureBuffer)
    {
        UnloadTexture(texture.second);
    }

    textureBuffer.clear();
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
        throw invalid_argument("Given textureName doesn't give a valid texture!");
    }

    auto& iteratorPair = *iteratorMap;
    
    return iteratorPair.second;
}

void AssetManager::fillBuffer(const string& folderName, const int strNamePos)
{
    vector<string> fileNames;
    for(auto& entry : filesystem::directory_iterator(folderName))
    {
        fileNames.push_back(entry.path().string());
    }

    for(auto& fileName : fileNames)
    {
        const char* texturePath = fileName.c_str();
        string textureName = fileName.substr(strNamePos);
        Texture2D texture = LoadTexture(texturePath);
        textureBuffer.insert({textureName, texture});
    }
}
#pragma once

#include <raylib.h>

#include <string>
#include <mutex>
#include <unordered_map>

class AssetManager
{
private:
    static AssetManager* assetManagerInstance_;
    static std::mutex mutex_;
    std::unordered_map<std::string, Texture2D> uiTextureBuffer;

protected:
    AssetManager();
    ~AssetManager();

public:
    AssetManager(AssetManager &other) = delete;
    void operator=(const AssetManager &) = delete;
    static AssetManager* GetInstance();
    static void DeleteInstance();
    Texture2D GetTextureUI(const std::string& uiTextureName);
};
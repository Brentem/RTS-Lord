#pragma once

#include <raylib.h>

#include <string>
#include <mutex>
#include <unordered_map>

typedef std::unordered_map<std::string, Texture2D> TextureBuffer;

class AssetManager
{
private:
    static AssetManager* assetManagerInstance_;
    static std::mutex mutex_;
    TextureBuffer textureBuffer;

    void fillBuffer(const std::string& folderName, const int strNamePos);

protected:
    AssetManager();
    ~AssetManager();

public:
    AssetManager(AssetManager &other) = delete;
    void operator=(const AssetManager &) = delete;
    static AssetManager* GetInstance();
    static void DeleteInstance();
    Texture2D& GetTexture(const std::string& textureName);
};
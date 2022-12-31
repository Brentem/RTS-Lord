#pragma once

#include <raylib.h>

#include <string>

void LoadAssets();
void UnloadAssets();
Texture2D* GetTexture(const std::string& textureName);
Texture2D* GetTextureUI(const std::string& uiName);
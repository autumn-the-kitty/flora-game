#pragma once

#include "gameConfig.hpp"

#include <raylib.h>
#include <string>

class Sprite {
public:
    std::string name;
    Texture texture;
    void load(std::string imagePath)
    {
        name = imagePath;

        texture = LoadTexture(name.c_str());

        texture.height = GetScreenWidth() / WIDTH;
        texture.width = GetScreenWidth() / WIDTH;
    }
};

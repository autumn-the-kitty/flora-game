#pragma once

#include "gameConfig.hpp"
#include "sprite.cpp"

#include <cstdint>
#include <optional>
#include <raylib.h>

class Block {
public:
    Sprite sprite;
    bool isFatal;
    bool isPlank;
    Rectangle area;
};

class Terrain {
    Block grass;
    Block spike;
    Block rock;
    Block plank;

public:
    std::optional<Block> raw[(int)WIDTH][(int)HEIGHT];

    void initSprites()
    {
        Sprite grassSprite;
        grassSprite.load("resources/grass.png");
        grass = Block {
            .sprite = grassSprite,
            .isFatal = false,
        };

        Sprite spikeSprite;
        spikeSprite.load("resources/spike.png");
        spike = Block {
            .sprite = spikeSprite,
            .isFatal = true,
        };

        Sprite rockSprite;
        rockSprite.load("resources/rock.png");
        rock = Block {
            .sprite = rockSprite,
            .isFatal = false,
        };

        Sprite plankSprite;
        plankSprite.load("resources/plank.png");
        plank = Block {
            .sprite = plankSprite,
            .isFatal = false,
            .isPlank = true,
        };
    }

    Terrain initLevel(const uint8_t* level)
    {
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                switch (level[i + (int)WIDTH * j]) {
                case NULL_BLOCK:
                    raw[i][j] = std::nullopt;
                    break;
                case GRASS:
                    raw[i][j] = std::make_optional(grass);
                    break;
                case SPIKE:
                    raw[i][j] = std::make_optional(spike);
                    break;
                case ROCK:
                    raw[i][j] = std::make_optional(rock);
                    break;
                case PLANK:
                    raw[i][j] = std::make_optional(plank);
                    break;
                default:
                    raw[i][j] = std::nullopt;
                    break;
                }

                if (raw[i][j].has_value()) {
                    raw[i][j].value().area.width = (int)raw[i][j].value().sprite.texture.width;
                    if (!raw[i][j].value().isPlank) {
                        raw[i][j].value().area.height = (int)raw[i][j].value().sprite.texture.height;
                    } else {
                        raw[i][j].value().area.height = (int)raw[i][j].value().sprite.texture.height / 2.0;
                    }
                    raw[i][j].value().area.x = (int)(i * raw[i][j].value().sprite.texture.width);
                    raw[i][j].value().area.y = (int)(j * raw[i][j].value().sprite.texture.height);
                }
            }
        }
        return *this;
    }

    void draw()
    {
        for (unsigned int i = 0; i < WIDTH; i++) {
            for (unsigned int j = 0; j < HEIGHT; j++) {
                std::optional<Block> block = raw[i][j];
                if (block.has_value()) {
                    Texture blockTexture = block.value().sprite.texture;
                    int blockWidth = block.value().area.width;
                    int blockHeight = block.value().area.height;

                    DrawTexture(blockTexture, blockWidth * i, blockWidth * j, WHITE);

#ifdef DEBUG_MODE
                    DrawRectangleLinesEx(block.value().area, 3, PINK);
#endif
                }
            }
        }
    }
};

#pragma once

#include "gameConfig.hpp"
#include "sprite.cpp"
#include "terrain.cpp"

#include <chrono>
#include <optional>
#include <raylib.h>

class JumpState {
public:
    float yVelocity;
    bool isJumping;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
};

class PlayerState {
public:
    JumpState jumpState;
    bool cameInFromRight;
    bool cameInFromLeft;
    bool cameInFromUp;
    bool cameInFromDown;
};

class Player {
public:
    PlayerState state;
    Sprite sprite;
    Rectangle area;

    void die()
    {
        state.jumpState.yVelocity = 0.0;
        if (state.cameInFromRight) {
            area.x = GetScreenWidth() - area.width;
            area.y = GetScreenWidth() / WIDTH * 8.0f;
        } else if (state.cameInFromLeft) {
            area.x = 0.0;
            area.y = GetScreenWidth() / WIDTH * 8.0f;
        } else if (state.cameInFromUp) {
            area.x = GetScreenWidth() / 2.0;
            area.y = 0.0;
        } else if (state.cameInFromDown) {
            area.x = GetScreenWidth() / 2.0;
            area.y = 0.0;
        }
    }

    bool checkRightLevel()
    {
        return area.x < 0.0;
    }

    bool checkLeftLevel()
    {
        return area.x + area.width > GetScreenWidth();
    }

    bool checkUpLevel()
    {
        return area.y + area.height > GetScreenHeight();
    }

    bool checkDownLevel()
    {
        return area.y < 0.0;
    }

    bool checkDeath()
    {
        return area.y + area.height >= GetScreenHeight();
    }

    void init()
    {
        state.cameInFromLeft = true;
        sprite.load("resources/player.png");
        area = Rectangle {
            .x = 0.0,
            .y = GetScreenWidth() / WIDTH * 8.0f,
            .width = (float)(int)(GetScreenWidth() / WIDTH),
            .height = (float)(int)(GetScreenWidth() / WIDTH),
        };
    }

    void draw()
    {
        DrawTexture(sprite.texture, area.x, area.y, WHITE);
#ifdef DEBUG_MODE
        DrawLineEx(
            Vector2 { .x = area.x + area.width / 2.0f, .y = 0.0 },
            Vector2 { .x = area.x + area.width / 2.0f, .y = (float)GetScreenHeight() },
            1, WHITE);
        DrawLineEx(
            Vector2 { .x = 0.0, .y = area.y + area.height / 2.0f },
            Vector2 { .x = (float)GetScreenWidth(), .y = area.y + area.height / 2.0f },
            1, WHITE);

        DrawRectangleLinesEx(area, 3, BLUE);
#endif
    }

    void move(Terrain terrain)
    {
        bool collidesA = false;
        Rectangle nextAreaA = area;
        nextAreaA.x -= PLAYER_SPEED * GetFrameTime();

        bool collidesD = false;
        Rectangle nextAreaD = area;
        nextAreaD.x += PLAYER_SPEED * GetFrameTime();

        bool collidesSpace = false;
        Rectangle nextAreaSpace = area;
        nextAreaSpace.y += state.jumpState.yVelocity * GetFrameTime();

        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                std::optional<Block> block_o = terrain.raw[i][j];
                if (!block_o.has_value()) {
                    continue;
                }

                bool isFatal = block_o.value().isFatal;
                Rectangle blockArea = block_o.value().area;

                if (CheckCollisionRecs(nextAreaSpace, blockArea) && !isFatal) {
                    collidesSpace = true;

                    if (nextAreaSpace.y - area.y < 0) {
                        if (block_o.value().isPlank) {
                            area.y = nextAreaSpace.y;
                            state.jumpState.yVelocity += JUMP_Y_ACCELERATION * GetFrameTime();
                            return;
                        }
                        area.y = blockArea.y + blockArea.height;
                        state.jumpState.yVelocity = 0.0;
                    } else {
                        state.jumpState.isJumping = false;
                        area.y = blockArea.y - area.height;
                        state.jumpState.yVelocity = 0.0;
                    }
                }

                if (CheckCollisionRecs(nextAreaA, blockArea) && !isFatal && !block_o.value().isPlank) {
                    collidesA = true;
                    area.x = blockArea.x + blockArea.width;
                }

                if (CheckCollisionRecs(nextAreaD, blockArea) && !isFatal && !block_o.value().isPlank) {
                    collidesD = true;
                    area.x = blockArea.x - area.width;
                }

                if (CheckCollisionRecs(area, blockArea) && isFatal) {
                    die();
                    return;
                }
            }
        }

        if (!collidesD && IsKeyDown(KEY_D)) {
            area.x += PLAYER_SPEED * GetFrameTime();
        }

        if (!collidesA && IsKeyDown(KEY_A)) {
            area.x -= PLAYER_SPEED * GetFrameTime();
        }

        if (!collidesSpace) {
            state.jumpState.isJumping = true;
            area.y += state.jumpState.yVelocity * GetFrameTime();
            state.jumpState.yVelocity += JUMP_Y_ACCELERATION * GetFrameTime();
        }

        double timeSinceLastJump = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - state.jumpState.lastTime).count();
        if (IsKeyDown(KEY_SPACE) && !state.jumpState.isJumping && timeSinceLastJump > 200.0) {
            state.jumpState.lastTime = std::chrono::high_resolution_clock::now();
            state.jumpState.isJumping = true;
            state.jumpState.yVelocity = JUMP_Y_INITIAL_VELOCITY;
        }
    }
};

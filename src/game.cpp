#pragma once

#include "level.hpp"
#include "player.cpp"
#include "terrain.cpp"

#include <cstdio>
#include <optional>
#include <raylib.h>

class Game {
private:
    Player player;
    Level level;
    void loop()
    {
        player.move(level.terrain);

        if (player.area.x + player.area.width > GetScreenWidth() && !level.levelRight.has_value()) {
            player.die();
        } else if (player.area.x < 0.0 && !level.levelLeft.has_value()) {
            player.die();
        } else if (player.area.y + player.area.height > GetScreenHeight() && !level.levelDown.has_value()) {
            player.die();
        } else if (player.area.y < 0.0 && !level.levelUp.has_value()) {
            player.die();
        }

        if (player.checkRightLevel()) {
            if (level.levelLeft.has_value()) {
                level = *level.levelLeft.value();
                player.area.x = GetScreenWidth() - player.area.width;
                player.state.cameInFromRight = true;
                player.state.cameInFromLeft = false;
                player.state.cameInFromUp = false;
                player.state.cameInFromDown = false;
            }
        } else if (player.checkLeftLevel()) {
            if (level.levelRight.has_value()) {
                level = *level.levelRight.value();
                player.area.x = 0.0;
                player.state.cameInFromRight = false;
                player.state.cameInFromLeft = true;
                player.state.cameInFromUp = false;
                player.state.cameInFromDown = false;
            }
        } else if (player.checkDownLevel()) {
            if (level.levelUp.has_value()) {
                level = *level.levelUp.value();
                player.state.cameInFromRight = false;
                player.state.cameInFromLeft = false;
                player.state.cameInFromUp = true;
                player.state.cameInFromDown = false;
            }
        } else if (player.checkUpLevel()) {
            if (level.levelDown.has_value()) {
                level = *level.levelDown.value();
                player.area.y = 0.0;
                player.state.cameInFromRight = false;
                player.state.cameInFromLeft = false;
                player.state.cameInFromUp = false;
                player.state.cameInFromDown = true;
            }
        }

        BeginDrawing();
        DrawTexture(level.backgroundTexture, 0, 0, WHITE);
        level.terrain.draw();
        player.draw();

#ifdef DEBUG_MODE
#endif
        char buf[8];
        sprintf(buf, "%d", GetFPS());
        DrawText(buf, WIDTH / 2.0, HEIGHT / 2.0, 100, BLACK);

        EndDrawing();
    };

    void configureRaylib()
    {
#ifndef DEBUG_MODE
        SetConfigFlags(FLAG_VSYNC_HINT);
#endif
        SetExitKey(KEY_NULL);
        ToggleFullscreen();
        HideCursor();
    }

public:
    void start(std::string title)
    {
        InitWindow(0, 0, title.c_str());
        configureRaylib();

        player.init();

        Texture backgroundTexture = LoadTexture("resources/background.png");
        backgroundTexture.height = GetScreenHeight();
        backgroundTexture.width = GetScreenWidth();

        Terrain terrain;
        terrain.initSprites();

        Level level1Level = Level {
            .terrain = terrain.initLevel(level1),
            .backgroundTexture = backgroundTexture,
        };

        Level level2Level = Level {
            .levelLeft = std::make_optional(&level1Level),
            .terrain = terrain.initLevel(level2),
            .backgroundTexture = backgroundTexture,
        };

        level1Level.levelRight = std::make_optional(&level2Level);

        Level level3Level = Level {
            .levelLeft = std::make_optional(&level2Level),
            .terrain = terrain.initLevel(level3),
            .backgroundTexture = backgroundTexture,
        };

        level2Level.levelRight = std::make_optional(&level3Level);

        Level level4Level = Level {
            .levelUp = std::make_optional(&level3Level),
            .terrain = terrain.initLevel(level4),
            .backgroundTexture = backgroundTexture,
        };

        level3Level.levelDown = std::make_optional(&level4Level);

        level = level1Level;

        while (!WindowShouldClose()) {
            loop();
        }
        CloseWindow();
    }
};

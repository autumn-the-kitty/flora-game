#pragma once

// Turn this on/off to turn on/off debug mode
#define DEBUG_MODE

// Speeds for player movement
#define PLAYER_SPEED ((int)(GetScreenWidth() / 4.0))
#define FALL_SPEED 1200.0
#define JUMP_Y_ACCELERATION 7000.0
#define JUMP_Y_INITIAL_VELOCITY (-1700.0)

// Number of blocks per screen
#define WIDTH 32.0f
#define HEIGHT 18.0f

// Block definitions
#define NULL_BLOCK 0
#define GRASS 1
#define SPIKE 2
#define ROCK 3
#define PLANK 4

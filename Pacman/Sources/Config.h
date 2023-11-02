#pragma once
#define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 962;
const int DOTS_SCORE = 10;
const int POWER_SCORE = 50;
const int LIFE_SCORE = 10000;

const int GHOST_SFX_CHANNEL = 2;

#if _DEBUG

#define CREATE_DEBUG_CONSOLE 1
#define PAUSE_CONSOLE_WHEN_CLOSING 0
#define SHOW_COLLISION 0
#define STARTING_LIFE 2
#define START_AT_ROUND 1
#define START_IN_GAME 0
#define DEBUG_PLAYER_MOVEMENTS 0
#define INVINSIBLE 0

#else

#define CREATE_DEBUG_CONSOLE 0
#define PAUSE_CONSOLE_WHEN_CLOSING 0
#define SHOW_COLLISION 0
#define STARTING_LIFE 2
#define START_AT_ROUND 1
#define START_IN_GAME 0
#define DEBUG_PLAYER_MOVEMENTS 0
#define INVINSIBLE 0

#endif
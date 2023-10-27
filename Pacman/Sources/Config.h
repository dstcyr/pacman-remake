#pragma once
#define SAFE_DELETE(ptr) if(ptr) { delete ptr; ptr = nullptr; }

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 962;

#define CREATE_DEBUG_CONSOLE 1
#define PAUSE_CONSOLE_WHEN_CLOSING 0
#define SHOW_COLLISION 0
#define STARTING_LIFE 2
#define START_AT_ROUND 1
#define START_IN_GAME 1
#define DEBUG_PLAYER_MOVEMENTS 0


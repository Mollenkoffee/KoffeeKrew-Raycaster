#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

const float PI = 3.14159265358979323846f;
const float NO_HIT_DISTANCE = 1e30f;
const float FOV = PI / 3;
const int MAX_DEPTH_OF_FIELD = 8;
const float PLAYER_MOVE_SPEED = 100.0f;
const float PLAYER_ROTATE_SPEED = 2.0f;
const int MAP_X_UNITS = 8;
const int MAP_Y_UNITS = 8;
const int MAP_UNIT_SIZE = 64;

#endif // CONSTANTS_H
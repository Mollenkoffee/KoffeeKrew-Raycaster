#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "Player.h"
#include "Map.h"
#include "Constants.h"

class Raycaster 
{
    public:
        void drawRays3D(const Player& player, const Map& map, int windowWidth, int windowHeight) const;

    private:
        float castRayHorizontal(float rayAngle, float playerX, float playerY, const Map& map, float& hitX, float& hitY) const;
        float castRayVertical(float rayAngle, float playerX, float playerY, const Map& map, float& hitX, float& hitY) const;
        void drawWallSlice(float x, float wallHeight, bool isVertical) const;
};

#endif // RAYCASTER_H
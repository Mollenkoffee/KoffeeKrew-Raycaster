#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "Player.h"
#include "Map.h"
#include "Constants.h"
#include "Texture.h"

class Raycaster 
{
    public:
        void drawRays3D(const Player& player, const Map& map, int windowWidth, int windowHeight, const Texture& texture) const;

    private:
        float castRayHorizontal(float rayAngle, float playerX, float playerY, const Map& map, float& hitX, float& hitY) const;
        float castRayVertical(float rayAngle, float playerX, float playerY, const Map& map, float& hitX, float& hitY) const;
        void drawWallSlice(float x, float wallHeight, bool isVertical, int textureIndex, float hitX, float hitY, const Texture& texture, Map::TileType tileType) const;
        void drawCeilingSlice(float x, float wallTop, float rayAngle, float playerDist, const Player& player, const Texture& texture) const;
        void drawFloorSlice(float x, float wallBottom, float rayAngle, float playerDist, const Player& player, const Texture& texture) const;
};

#endif // RAYCASTER_H
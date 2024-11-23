#include "Raycaster.h"
#include <GL/glew.h>

float Raycaster::castRayHorizontal(float rayAngle, float playerX, float playerY, const Map& map, float& hitX, float& hitY) const 
{
    rayAngle = fmod(rayAngle + 2 * PI, 2 * PI); // Normalize angle
    float inverseSlope = -1.0f / tan(rayAngle);
    float rayStepX, rayStepY;
    int depthOfField = 0;

    if (rayAngle > PI) 
    {
        hitY = ((int)(playerY / Map::mapUnitSize)) * Map::mapUnitSize - 0.0001f;
        hitX = playerX + (playerY - hitY) * inverseSlope;
        rayStepY = -Map::mapUnitSize;
        rayStepX = -rayStepY * inverseSlope;
    }
    else if (rayAngle < PI) 
    {
        hitY = ((int)(playerY / Map::mapUnitSize)) * Map::mapUnitSize + Map::mapUnitSize;
        hitX = playerX + (playerY - hitY) * inverseSlope;
        rayStepY = Map::mapUnitSize;
        rayStepX = -rayStepY * inverseSlope;
    }
    else 
    {
        return NO_HIT_DISTANCE; // Ray is perfectly horizontal
    }

    while (depthOfField < MAX_DEPTH_OF_FIELD) 
    {
        int mapGridX = (int)(hitX / Map::mapUnitSize);
        int mapGridY = (int)(hitY / Map::mapUnitSize);

        if (map.isColliding(hitX, hitY))
        {
            return sqrt(pow(hitX - playerX, 2) + pow(hitY - playerY, 2));
        }

        hitX += rayStepX;
        hitY += rayStepY;
        ++depthOfField;
    }

    return NO_HIT_DISTANCE;
}

float Raycaster::castRayVertical(float rayAngle, float playerX, float playerY, const Map& map, float& hitX, float& hitY) const 
{
    rayAngle = fmod(rayAngle + 2 * PI, 2 * PI); // Normalize angle
    float slope = -tan(rayAngle);
    float rayStepX, rayStepY;
    int depthOfField = 0;

    if (rayAngle > PI / 2 && rayAngle < 3 * PI / 2) 
    {
        hitX = ((int)(playerX / Map::mapUnitSize)) * Map::mapUnitSize - 0.0001f;
        hitY = playerY + (playerX - hitX) * slope;
        rayStepX = -Map::mapUnitSize;
        rayStepY = -rayStepX * slope;
    }
    else 
    {
        hitX = ((int)(playerX / Map::mapUnitSize)) * Map::mapUnitSize + Map::mapUnitSize;
        hitY = playerY + (playerX - hitX) * slope;
        rayStepX = Map::mapUnitSize;
        rayStepY = -rayStepX * slope;
    }

    while (depthOfField < MAX_DEPTH_OF_FIELD) 
    {
        int mapGridX = (int)(hitX / Map::mapUnitSize);
        int mapGridY = (int)(hitY / Map::mapUnitSize);

        if (map.isColliding(hitX, hitY))
        {
            return sqrt(pow(hitX - playerX, 2) + pow(hitY - playerY, 2));
        }

        hitX += rayStepX;
        hitY += rayStepY;
        ++depthOfField;
    }

    return NO_HIT_DISTANCE;
}

void Raycaster::drawWallSlice(float x, float wallHeight, bool isVertical) const 
{
    float wallTop = (WINDOW_HEIGHT / 2.0f) - (wallHeight / 2.0f);
    float wallBottom = (WINDOW_HEIGHT / 2.0f) + (wallHeight / 2.0f);
    float wallSliceWidth = static_cast<float>(WINDOW_WIDTH) / 60.0f;

    if (isVertical)
    {
        glColor3f(0.6f, 0.6f, 0.6f);
    }
    else
    {
        glColor3f(0.8f, 0.8f, 0.8f);
    }

    glBegin(GL_QUADS);
    glVertex2f(x, wallTop);
    glVertex2f(x + wallSliceWidth, wallTop);
    glVertex2f(x + wallSliceWidth, wallBottom);
    glVertex2f(x, wallBottom);
    glEnd();
}

void Raycaster::drawRays3D(const Player& player, const Map& map, int windowWidth, int windowHeight) const 
{
    float rayAngle = player.angle - (FOV / 2);

    for (int rayIndex = 0; rayIndex < 60; ++rayIndex) 
    {
        float horizontalHitX, horizontalHitY, verticalHitX, verticalHitY;

        float horizontalDistance = castRayHorizontal(rayAngle, player.x, player.y, map, horizontalHitX, horizontalHitY);
        float verticalDistance = castRayVertical(rayAngle, player.x, player.y, map, verticalHitX, verticalHitY);

        bool isVerticalHit = verticalDistance < horizontalDistance;
        float rayDistance = isVerticalHit ? verticalDistance : horizontalDistance;

        rayDistance *= cos(player.angle - rayAngle); // Correct fisheye distortion
        float wallHeight = (Map::mapUnitSize * (windowHeight / 2.0f)) / rayDistance;

        drawWallSlice(rayIndex * (windowWidth / 60.0f), wallHeight, isVerticalHit);
        rayAngle += FOV / 60.0f;
    }
}
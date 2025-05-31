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

void Raycaster::drawWallSlice(float x, float wallHeight, bool isVertical, int textureIndex, float hitX, float hitY, const Texture& texture, Map::TileType tileType) const 
{
    float wallTop = (WINDOW_HEIGHT / 2.0f) - (wallHeight / 2.0f);
    float wallBottom = (WINDOW_HEIGHT / 2.0f) + (wallHeight / 2.0f);
    float wallSliceWidth = static_cast<float>(WINDOW_WIDTH) / 60.0f;

    int textureColumn = static_cast<int>(isVertical ? fmod(hitY, Texture::TEXTURE_SIZE) : fmod(hitX, Texture::TEXTURE_SIZE));

    // Avoid out-of-bounds errors
    textureColumn = std::max(0, std::min(Texture::TEXTURE_SIZE - 1, textureColumn));

    // Apply shading based on hit direction
    float shadingFactor = isVertical ? 0.6f : 1.0f;

    for (int y = static_cast<int>(wallTop); y < static_cast<int>(wallBottom); ++y) 
    {
        int textureRow = static_cast<int>(((y - wallTop) / wallHeight) * Texture::TEXTURE_SIZE);
        textureRow = std::max(0, std::min(Texture::TEXTURE_SIZE - 1, textureRow));

        int pixel = texture.getPixel(textureIndex, textureColumn, textureRow);

        float colorIntensity = (pixel == 1) ? shadingFactor : shadingFactor * 0.5f;

        float r = 1.0f;
        float g = 1.0f; 
        float b = 1.0f;
        
        switch (tileType)
        {
        case Map::TileType::WALL:
            r = 1.0f;
            g = 0.0f;
            b = 0.0f; 
            break;
        case Map::TileType::DOOR_CLOSED:
            r = 0.0f; 
            g = 1.0f; 
            b = 0.0f;
            break;
        case Map::TileType::DOOR_OPEN:
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            break;
        default:
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            break;
        }

        float finalR = r * shadingFactor * colorIntensity;
        float finalG = g * shadingFactor * colorIntensity;
        float finalB = b * shadingFactor * colorIntensity;

        glColor3f(finalR, finalG, finalB);

        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + wallSliceWidth, y);
        glVertex2f(x + wallSliceWidth, y + 1);
        glVertex2f(x, y + 1);
        glEnd();
    }
}

void Raycaster::drawRays3D(const Player& player, const Map& map, int windowWidth, int windowHeight, const Texture& texture) const
{
    float rayAngle = player.angle - (FOV / 2);

    for (int rayIndex = 0; rayIndex < 60; ++rayIndex)
    {
        float horizontalHitX, horizontalHitY, verticalHitX, verticalHitY;
        float horizontalDistance = castRayHorizontal(rayAngle, player.x, player.y, map, horizontalHitX, horizontalHitY);
        float verticalDistance = castRayVertical(rayAngle, player.x, player.y, map, verticalHitX, verticalHitY);

        bool isVerticalHit = verticalDistance < horizontalDistance;
        float hitX = isVerticalHit ? verticalHitX : horizontalHitX;
        float hitY = isVerticalHit ? verticalHitY : horizontalHitY;
        float rayDistance = isVerticalHit ? verticalDistance : horizontalDistance;

        // Correct fisheye distortion
        rayDistance *= cos(player.angle - rayAngle);
        float wallHeight = (Map::mapUnitSize * (windowHeight / 2.0f)) / rayDistance;

        // Convert hit point to grid coordinates
        int tileX = static_cast<int>(hitX) / Map::mapUnitSize;
        int tileY = static_cast<int>(hitY) / Map::mapUnitSize;

        // Default to wall texture
        int textureIndex = 2;

        // Look up tile type from map
        int tileType = map.getTile(tileX, tileY); 

        if (tileType == Map::TileType::WALL)
        {
            textureIndex = 2;
        }
        else if (tileType == Map::TileType::DOOR_CLOSED)
        {
            textureIndex = 3;
        }

		
		Map::TileType tileTypeEnum = static_cast<Map::TileType>(tileType);

        drawWallSlice(rayIndex * (windowWidth / 60.0f), wallHeight, isVerticalHit, textureIndex, hitX, hitY, texture, tileTypeEnum);

        rayAngle += FOV / 60.0f;
    }
}
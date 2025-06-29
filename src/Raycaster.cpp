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

    int textureColumn = static_cast<int>(isVertical ? fmod(hitY, TEXTURE_SIZE) : fmod(hitX, TEXTURE_SIZE));

    // Avoid out-of-bounds errors
    textureColumn = std::max(0, std::min(TEXTURE_SIZE - 1, textureColumn));

    // Apply shading based on hit direction
    float shadingFactor = isVertical ? 0.6f : 1.0f;

    for (int y = static_cast<int>(wallTop); y < static_cast<int>(wallBottom); ++y) 
    {
        int textureRow = static_cast<int>(((y - wallTop) / wallHeight) * TEXTURE_SIZE);
        textureRow = std::max(0, std::min(TEXTURE_SIZE - 1, textureRow));

        if (TEXTURE_MODE == TextureMode::ARRAY)
        {
            int pixel = texture.getPixelArrayTexture(textureIndex, textureColumn, textureRow);

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
        }
		else if (TEXTURE_MODE == TextureMode::FILE)
		{
            int pixel = texture.getPixelFileTexture(textureIndex, textureColumn, textureRow);

            float r = ((pixel >> 16) & 0xFF) / 255.0f;
            float g = ((pixel >> 8) & 0xFF) / 255.0f;
            float b = (pixel & 0xFF) / 255.0f;

            r *= shadingFactor;
            g *= shadingFactor;
            b *= shadingFactor;

            glColor3f(r, g, b);
		}
        
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + wallSliceWidth, y);
        glVertex2f(x + wallSliceWidth, y + 1);
        glVertex2f(x, y + 1);
        glEnd();
    }
}

void Raycaster::drawCeilingSlice(float x, float wallTop, float rayAngle, float playerDist, const Player& player, const Texture& texture) const
{
    float wallSliceWidth = static_cast<float>(WINDOW_WIDTH) / 60.0f;

    for (int y = 0; y < static_cast<int>(wallTop); ++y)
    {
        float distance = (Map::mapUnitSize * 0.5f * WINDOW_HEIGHT) / (WINDOW_HEIGHT / 2.0f - y);
        float worldX = player.x + distance * cos(rayAngle);
        float worldY = player.y + distance * sin(rayAngle);

        int tileX = static_cast<int>(worldX) / Map::mapUnitSize;
        int tileY = static_cast<int>(worldY) / Map::mapUnitSize;
        
        // Ensure texture coordinates are always positive and wrap correctly.
        int textureX = ((static_cast<int>(worldX) % TEXTURE_SIZE) + TEXTURE_SIZE) % TEXTURE_SIZE;
        int textureY = ((static_cast<int>(worldY) % TEXTURE_SIZE) + TEXTURE_SIZE) % TEXTURE_SIZE;

        textureX = std::max(0, std::min(TEXTURE_SIZE - 1, textureX));
        textureY = std::max(0, std::min(TEXTURE_SIZE - 1, textureY));

        if (TEXTURE_MODE == TextureMode::ARRAY)
        {
            int pixel = texture.getPixelArrayTexture(TEX_ARRAY_TILE_PATTERN_1, textureX, textureY);

            float baseIntensity = (pixel == 0) ? 0.6f : 1.0f;

            float r = baseIntensity * 0.5f;
            float g = baseIntensity * 0.5f;
            float b = baseIntensity * 0.0f;

            glColor3f(r, g, b);
        }
        else if (TEXTURE_MODE == TextureMode::FILE)
        {
            int pixel = texture.getPixelFileTexture(TEX_FILE_CEILING, textureX, textureY);

            float r = ((pixel >> 16) & 0xFF) / 255.0f;
            float g = ((pixel >> 8) & 0xFF) / 255.0f;
            float b = (pixel & 0xFF) / 255.0f;

            glColor3f(r, g, b);
        }
        
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + wallSliceWidth, y);
        glVertex2f(x + wallSliceWidth, y + 1);
        glVertex2f(x, y + 1);
        glEnd();
    }
}

void Raycaster::drawFloorSlice(float x, float wallBottom, float rayAngle, float playerDist, const Player& player, const Texture& texture) const
{
    float wallSliceWidth = static_cast<float>(WINDOW_WIDTH) / 60.0f;

    for (int y = static_cast<int>(wallBottom); y < WINDOW_HEIGHT; ++y)
    {
        float distance = (Map::mapUnitSize * 0.5f * WINDOW_HEIGHT) / (y - WINDOW_HEIGHT / 2.0f);
        float worldX = player.x + distance * cos(rayAngle);
        float worldY = player.y + distance * sin(rayAngle);

        int tileX = static_cast<int>(worldX) / Map::mapUnitSize;
        int tileY = static_cast<int>(worldY) / Map::mapUnitSize;

        // Ensure texture coordinates are always positive and wrap correctly.
        int textureX = ((static_cast<int>(worldX) % TEXTURE_SIZE) + TEXTURE_SIZE) % TEXTURE_SIZE;
        int textureY = ((static_cast<int>(worldY) % TEXTURE_SIZE) + TEXTURE_SIZE) % TEXTURE_SIZE;

        textureX = std::max(0, std::min(TEXTURE_SIZE - 1, textureX));
        textureY = std::max(0, std::min(TEXTURE_SIZE - 1, textureY));

        if (TEXTURE_MODE == TextureMode::ARRAY)
        {
            int pixel = texture.getPixelArrayTexture(TEX_ARRAY_TILE_PATTERN_2, textureX, textureY);

            float baseIntensity = (pixel == 1) ? 0.6f : 1.0f;

            float r = baseIntensity * 0.0f;
            float g = baseIntensity * 0.5f;
            float b = baseIntensity * 0.5f;

			glColor3f(r, g, b);
		}
		else if (TEXTURE_MODE == TextureMode::FILE)
		{
			int pixel = texture.getPixelFileTexture(TEX_FILE_FLOOR, textureX, textureY);

			float r = ((pixel >> 16) & 0xFF) / 255.0f;
			float g = ((pixel >> 8) & 0xFF) / 255.0f;
			float b = (pixel & 0xFF) / 255.0f;

			glColor3f(r, g, b);
        }
        
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

        // Look up tile type from map
        int tileType = map.getTile(tileX, tileY); 

        int textureIndex = 0;

        if (TEXTURE_MODE == TextureMode::ARRAY)
        {
            if (tileType == Map::TileType::WALL)
            {
                textureIndex = TEX_ARRAY_BRICK;
            } 
            else if (tileType == Map::TileType::DOOR_CLOSED)
            {
                textureIndex = TEX_ARRAY_SQUARE_PATTERN;
            }
            else
            {
                textureIndex = TEX_ARRAY_DEFAULT;
            }
        }
        else if (TEXTURE_MODE == TextureMode::FILE)
        {
            if (tileType == Map::TileType::WALL)
            {
                textureIndex = TEX_FILE_WALL;
            }
            else if (tileType == Map::TileType::DOOR_CLOSED)
            {
                textureIndex = TEX_FILE_DOOR_CLOSED;
            }
            else
            {
                textureIndex = TEX_FILE_DEFAULT;
            }
        }
		
		Map::TileType tileTypeEnum = static_cast<Map::TileType>(tileType);

        drawWallSlice(rayIndex * (windowWidth / 60.0f), wallHeight, isVerticalHit, textureIndex, hitX, hitY, texture, tileTypeEnum);

        drawCeilingSlice(rayIndex * (windowWidth / 60.0f), (WINDOW_HEIGHT / 2.0f) - (wallHeight / 2.0f), rayAngle, rayDistance, player, texture);

        drawFloorSlice(rayIndex * (windowWidth / 60.0f), (WINDOW_HEIGHT / 2.0f) + (wallHeight / 2.0f), rayAngle, rayDistance, player, texture);

        rayAngle += FOV / 60.0f;
    }
}
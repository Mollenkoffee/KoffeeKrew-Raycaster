#include "Map.h"
#include <GL/glew.h>

void Map::drawMap2D(int windowWidth, int windowHeight) const
{
    float tileWidth = static_cast<float>(windowWidth) / mapXUnits;
    float tileHeight = static_cast<float>(windowHeight) / mapYUnits;

    for (int y = 0; y < mapYUnits; y++)
    {
        for (int x = 0; x < mapXUnits; x++)
        {
            int tile = map[y * mapXUnits + x];

            switch (tile)
            {
            // Wall
            case 1: 
                glColor3f(1.0f, 0.0f, 0.0f);
                break;
            // Closed Door
            case 2: 
                glColor3f(0.0f, 1.0f, 0.0f);
                break;
            // Open Door
            case 3: 
                glColor3f(1.0f, 1.0f, 1.0f);
                break;
            // Empty Space
            case 0: 
            default:
                glColor3f(1.0f, 1.0f, 1.0f);
                break;
            }

            float tileX = x * tileWidth;
            float tileY = y * tileHeight;

            glBegin(GL_QUADS);
            glVertex2f(tileX + 1, tileY + 1);
            glVertex2f(tileX + 1, tileY + tileHeight - 1);
            glVertex2f(tileX + tileWidth - 1, tileY + tileHeight - 1);
            glVertex2f(tileX + tileWidth - 1, tileY + 1);
            glEnd();
        }
    }
}

bool Map::isColliding(float x, float y) const
{
    int gridX = static_cast<int>(x) / mapUnitSize;
    int gridY = static_cast<int>(y) / mapUnitSize;

    if (gridX < 0 || gridX >= mapXUnits || gridY < 0 || gridY >= mapYUnits)
    {
        return true;
    }

    int tile = map[gridY * mapXUnits + gridX];
    
    // Wall or closed door
    return tile == 1 || tile == 2; 
}

void Map::toggleDoor(int gridX, int gridY) 
{
    int index = gridY * mapXUnits + gridX;

    if (map[index] == TileType::DOOR_CLOSED)
    {
        map[index] = TileType::DOOR_OPEN;
    } 
    else if (map[index] == TileType::DOOR_OPEN)
    {
        map[index] = TileType::DOOR_CLOSED;
    }  
}

bool Map::isDoor(int gridX, int gridY) const 
{
    if (gridX < 0 || gridX >= mapXUnits || gridY < 0 || gridY >= mapYUnits)
    {
        return false;
    }

    int tile = map[gridY * mapXUnits + gridX];
    return tile == TileType::DOOR_CLOSED || tile == TileType::DOOR_OPEN;
}

int Map::getTile(int x, int y) const
{
    if (x < 0 || x >= mapXUnits || y < 0 || y >= mapYUnits)
    {
        return TileType::EMPTY;
    }
        
    return map[y * mapXUnits + x];
}
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
            if (map[y * mapXUnits + x] == 1) 
            {
                glColor3f(0.0f, 0.0f, 0.0f); // Wall
            }
            else if (map[y * mapXUnits + x] == 0)
            {
                glColor3f(1.0f, 1.0f, 1.0f); // Empty space
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

    return map[gridY * mapXUnits + gridX] == 1;
}
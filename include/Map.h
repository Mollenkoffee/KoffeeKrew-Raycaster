#ifndef MAP_H
#define MAP_H

#include "Constants.h"

class Map 
{
    public:
        static const int mapXUnits = MAP_X_UNITS;
        static const int mapYUnits = MAP_Y_UNITS;
        static const int mapUnitSize = MAP_UNIT_SIZE;

        int map[mapYUnits * mapXUnits] = 
        {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 1, 0, 0, 1, 0, 1,
            1, 2, 1, 0, 0, 1, 2, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 2, 1, 0, 0, 1, 2, 1,
            1, 0, 1, 0, 0, 1, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1
        };

        enum TileType {
            EMPTY = 0,
            WALL = 1,
            DOOR_CLOSED = 2,
            DOOR_OPEN = 3
        };

        void drawMap2D(int windowWidth, int windowHeight) const;
        bool isColliding(float x, float y) const;
        void toggleDoor(int gridX, int gridY);
        bool isDoor(int gridX, int gridY) const;
		int getTile(int x, int y) const;
};

#endif // MAP_H
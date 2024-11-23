#ifndef MAP_H
#define MAP_H

#include "Constants.h"

class Map 
{
    public:
        static const int mapXUnits = MAP_X_UNITS;
        static const int mapYUnits = MAP_Y_UNITS;
        static const int mapUnitSize = MAP_UNIT_SIZE;

        const int map[mapYUnits * mapXUnits] = 
        {
            1, 1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 1, 1, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 1, 0, 1, 1, 0, 1, 1,
            1, 1, 0, 1, 1, 0, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 1,
            1, 0, 0, 1, 1, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1
        };

        void drawMap2D(int windowWidth, int windowHeight) const;
        bool isColliding(float x, float y) const;
};

#endif // MAP_H
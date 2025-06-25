#ifndef PLAYER_H
#define PLAYER_H

#include "Map.h"

class Player 
{
    public:
        float x, y, angle, deltaX, deltaY;

        Player(float startX, float startY, float startAngle);
        void move(float speed, const Map& map, int direction);
        void moveSideways(float speed, const Map& map, int direction);
        void moveAbsolute(float directionX, float directionY, const Map& map);
        void rotate(float angleDelta);
        void drawPlayer(int windowWidth, int windowHeight) const;
};

#endif // PLAYER_H
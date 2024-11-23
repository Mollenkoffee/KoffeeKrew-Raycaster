#include "Player.h"
#include <GL/glew.h>
#include <math.h>
#include "Constants.h"

Player::Player(float startX, float startY, float startAngle) : x(startX), y(startY), angle(startAngle) 
{
    deltaX = cos(angle);
    deltaY = sin(angle);
}

void Player::move(float speed, const Map& map, int direction) 
{
    float nextX = x + deltaX * speed * direction;
    float nextY = y + deltaY * speed * direction;
    
    if (!map.isColliding(nextX, nextY)) 
    {
        x = nextX;
        y = nextY;
    }
}

void Player::rotate(float angleDelta) 
{
    angle += angleDelta;
    
    while (angle < 0)
    {
        angle += 2 * PI;
    }

    while (angle > 2 * PI)
    {
        angle -= 2 * PI;
    }

    deltaX = cos(angle);
    deltaY = sin(angle);
}

void Player::drawPlayer(int windowWidth, int windowHeight) const 
{
    float scaleX = (float)windowWidth / (Map::mapXUnits * Map::mapUnitSize);
    float scaleY = (float)windowHeight / (Map::mapYUnits * Map::mapUnitSize);

    float playerScreenX = x * scaleX;
    float playerScreenY = y * scaleY;

    // Draw player position
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(playerScreenX, playerScreenY);
    glEnd();

    // Draw player direction
    glBegin(GL_LINES);
    glVertex2f(playerScreenX, playerScreenY);
    glVertex2f(playerScreenX + deltaX * 10 * scaleX, playerScreenY + deltaY * 10 * scaleY);
    glEnd();
}
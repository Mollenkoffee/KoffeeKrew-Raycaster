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
    
    const float collisionBuffer = 4.0f;

    float futureX = deltaX * direction > 0 ? nextX + collisionBuffer : nextX - collisionBuffer;
    float futureY = deltaY * direction > 0 ? nextY + collisionBuffer : nextY - collisionBuffer;

    if (!map.isColliding(futureX, y))
    {
        x = nextX;
    }

    if (!map.isColliding(x, futureY))
    {
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

    // Direction vector scaled
    float dirX = deltaX * 10 * scaleX;
    float dirY = deltaY * 10 * scaleY;

    // Arrowhead size
    float arrowSize = 5.0f;

    // Perpendicular vector for arrowhead
    float perpX = -dirY;
    float perpY = dirX;
    float length = sqrt(perpX * perpX + perpY * perpY);
    perpX = (perpX / length) * arrowSize;
    perpY = (perpY / length) * arrowSize;

    float endX = playerScreenX + dirX;
    float endY = playerScreenY + dirY;

    // Draw player position
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    glVertex2f(playerScreenX, playerScreenY);
    glEnd();

    // Draw main direction line
    glBegin(GL_LINES);
    glVertex2f(playerScreenX, playerScreenY);
    glVertex2f(endX, endY);
    glEnd();

    // Draw arrowhead
    glBegin(GL_LINES);
    glVertex2f(endX, endY);
    glVertex2f(endX - dirX * 0.2f + perpX, endY - dirY * 0.2f + perpY);

    glVertex2f(endX, endY);
    glVertex2f(endX - dirX * 0.2f - perpX, endY - dirY * 0.2f - perpY);
    glEnd();

    // Draw door interaction ray
    /*
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(playerScreenX, playerScreenY);
    glVertex2f(playerScreenX + deltaX * 32 * scaleX, playerScreenY + deltaY * 32 * scaleY);
    glEnd();
    */
}
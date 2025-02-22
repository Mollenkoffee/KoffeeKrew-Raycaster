#include "Renderer.h"
#include <GL/glew.h>

void Renderer::toggleRenderMap() 
{
    renderMap = !renderMap;
}

void Renderer::render(const Map& map, const Player& player, const Raycaster& raycaster, int windowWidth, int windowHeight, const Texture& texture)
{
    if (renderMap) 
    {
        map.drawMap2D(windowWidth, windowHeight);
        player.drawPlayer(windowWidth, windowHeight);
    }
    else 
    {
        raycaster.drawRays3D(player, map, windowWidth, windowHeight, texture);
    }
}
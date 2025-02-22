#ifndef RENDERER_H
#define RENDERER_H

#include "Map.h"
#include "Player.h"
#include "Raycaster.h"
#include "Constants.h"
#include "Texture.h"

class Renderer 
{
    public:
        void toggleRenderMap();
        void render(const Map& map, const Player& player, const Raycaster& raycaster, int windowWidth, int windowHeight, const Texture& texture);

    private:
        bool renderMap = true;
};

#endif // RENDERER_H
#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

class Texture 
{
    public:
        static const int TEXTURE_SIZE = 32;

        std::vector<int> textures;

        Texture();
        int getPixel(int textureIndex, int x, int y) const;
};

#endif // TEXTURE_H
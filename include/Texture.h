#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include "Constants.h"

enum ArrayTextureIndex 
{
    TEX_ARRAY_DEFAULT = 0,
    TEX_ARRAY_CHECKER = 1,
    TEX_ARRAY_BRICK = 2,
    TEX_ARRAY_SQUARE_PATTERN = 3,
    TEX_ARRAY_TILE_PATTERN_1 = 4,
    TEX_ARRAY_TILE_PATTERN_2 = 5
};

enum FileTextureIndex 
{
    TEX_FILE_DEFAULT = 0,
    TEX_FILE_WALL = 1,
    TEX_FILE_DOOR_CLOSED = 2,
    TEX_FILE_CEILING = 3,
    TEX_FILE_FLOOR = 4
};

class Texture 
{
    public:
        std::vector<int> arrayTextures;

        Texture();
        int getPixelArrayTexture(int textureIndex, int x, int y) const;
        int getPixelFileTexture(int textureIndex, int x, int y) const;
        bool loadFromFile(int textureIndex, const std::string& filename);
    
    private:
        // Each texture: RGB * 32 * 32
        std::vector<std::vector<unsigned char>> textureData;
};

#endif // TEXTURE_H
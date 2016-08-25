#ifndef __TEXTUREMGR_H
#define __TEXTUREMGR_H

/********************************************************************
* CTextureMgr
* This class provides a central location for loading and binding
* 2D textures.
********************************************************************/
class CTargaImage;

#define MAX_TEXTURES 64

// a texture object used in the texture manager
struct TextureObj {
    unsigned int textureObjectID;                   // texture object
    bool         inUse;                             // is this object loaded?
    char         textureName[28];                   // name of texture
};

class CTextureMgr
{
private:
    TextureObj   m_textureObjects[MAX_TEXTURES];    // array of textures
    int          m_numberTextures;                  // number of textures in array

    enum FilterModes {
        FILTER_NONE,
        FILTER_LOW,
        FILTER_MED,
        FILTER_HI,
        FILTER_BEST
    };

    enum WrapModes {
        WRAP_NONE,
        WRAP_REPEAT,
        WRAP_CLAMP,
        WRAP_CLAMPEDGE,
        WRAP_CLAMPBORDER,
        WRAP_CLAMPMIRROR
    };

    enum TexEnvs {
        ENV_NONE,
        ENV_REPLACE,
        ENV_MODULATE,
        ENV_DECAL,
        ENV_BLEND,
        ENV_ADD
    };

    int ConvertStrToFilter(char *str);
    int ConvertStrToWrap(char *str);
    int ConvertStrToTexEnv(char *str);

public:
    CTextureMgr();
    virtual ~CTextureMgr();

    // loads texture names from a data file
    bool LoadTextures(char *textureDataFile);

    // release all textures
    bool ReleaseTextures();

    // returns the texture's id for use in the texture manager
    unsigned int GetTextureID(char *textureName);

    // binds the texture
    bool Bind(unsigned int textureID);
};

#endif
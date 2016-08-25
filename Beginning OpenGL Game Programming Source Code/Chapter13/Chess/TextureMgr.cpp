#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <iostream>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"

#include "CTargaImage.h"
#include "TextureMgr.h"

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
}

int CTextureMgr::ConvertStrToFilter(char *str)
{
    if (!strcmp(strupr(str), "LOW"))
        return FILTER_LOW;
    else if (!strcmp(strupr(str), "MED"))
        return FILTER_MED;
    else if (!strcmp(strupr(str), "HI"))
        return FILTER_HI;
    else if (!strcmp(strupr(str), "BEST"))
        return FILTER_BEST;
    else
        return FILTER_LOW;
}

int CTextureMgr::ConvertStrToWrap(char *str)
{
    if (!strcmp(strupr(str), "REPEAT"))
        return GL_REPEAT;
    else if (!strcmp(strupr(str), "CLAMP"))
        return GL_CLAMP;
    else if (!strcmp(strupr(str), "CLAMPEDGE"))
        return GL_CLAMP_TO_EDGE;
    else if (!strcmp(strupr(str), "CLAMPBORDER"))
        return GL_CLAMP_TO_BORDER;
    else if (!strcmp(strupr(str), "MIRROR"))
        return GL_MIRRORED_REPEAT;
    else
        return GL_REPEAT;
}

int CTextureMgr::ConvertStrToTexEnv(char *str)
{
    if (!strcmp(strupr(str), "REPLACE"))
        return GL_REPLACE;
    else if (!strcmp(strupr(str), "MODULATE"))
        return GL_MODULATE;
    else if (!strcmp(strupr(str), "DECAL"))
        return GL_DECAL;
    else if (!strcmp(strupr(str), "BLEND"))
        return GL_BLEND;
    else if (!strcmp(strupr(str), "ADD"))
        return GL_ADD;
    else
        return GL_REPLACE;
}

bool CTextureMgr::LoadTextures(char *textureDataFile)
{
    // Texture data file format:
    // C++ style '//' at start of line is a comment
    // texturefilename   low/med/high/bestfiltermode
    FILE *fptr;
    fptr = fopen(textureDataFile, "r");

    if (!fptr)
        return false;

    m_numberTextures = 0;

    // read to EOF
    while ((!feof(fptr)) && (m_numberTextures < MAX_TEXTURES)) {
        char line[255];
        fgets(line, 255, fptr);

        // only read if this line is not a comment
        if ((line[0] != '/') && (line[1] != '/')) {
            char texName[30] = { '\0' };
            char filename[30] = { '\0' };
            char filterDetail[5] = { '\0' };
            char wrapMode[10] = { '\0' };
            char env[10] = { '\0' };
            sscanf(line, "%s %s %s %s %s", texName, filename, filterDetail, wrapMode, env);
            m_textureObjects[m_numberTextures].inUse = true;
            strcpy(m_textureObjects[m_numberTextures].textureName, texName);
            // load texture image data
            CTargaImage imgData;
            imgData.Load(filename);
            imgData.ConvertRGBToRGBA(255);
            // generate texture object
            glGenTextures(1, &(m_textureObjects[m_numberTextures].textureObjectID));
            // Make the texture the current one
            glBindTexture(GL_TEXTURE_2D, m_textureObjects[m_numberTextures].textureObjectID);
            int wrap = ConvertStrToWrap(wrapMode);
            int filter = ConvertStrToFilter(filterDetail);
            int texenv = ConvertStrToTexEnv(env);
            // Texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

            switch (filter) {
            case FILTER_LOW:
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.GetWidth(), imgData.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData.GetImage());
                break;

            case FILTER_MED:
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.GetWidth(), imgData.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData.GetImage());
                break;

            case FILTER_HI:
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);
                gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imgData.GetWidth(), imgData.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, imgData.GetImage());
                break;

            case FILTER_BEST:
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, texenv);
                gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imgData.GetWidth(), imgData.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, imgData.GetImage());
                break;
            }

            imgData.Release();
            m_numberTextures++;
        }
    }

    return true;
}

bool CTextureMgr::ReleaseTextures()
{
    for (int idx = 0; idx < m_numberTextures; idx++) {
        if (m_textureObjects[idx].inUse) {
            glDeleteTextures(1, &(m_textureObjects[idx].textureObjectID));
            m_textureObjects[idx].inUse = false;
        }
    }

    return true;
}

unsigned int CTextureMgr::GetTextureID(char *textureName)
{
    for (int idx = 0; idx < m_numberTextures; idx++) {
        if (!strcmp(textureName, m_textureObjects[idx].textureName))
            return idx;
    }

    return -1;
}

bool CTextureMgr::Bind(unsigned int textureID)
{
    if (m_textureObjects[textureID].inUse) {
        glBindTexture(GL_TEXTURE_2D, m_textureObjects[textureID].textureObjectID);
        return true;
    }

    return false;
}


#ifndef __GL_COMPONENT
#define __GL_COMPONENT


#include "glee.h"

const int TERRAIN_SIZE = 65;

class CGfxOpenGL
{
private:
    GLubyte heightmap[TERRAIN_SIZE * TERRAIN_SIZE];
    int m_windowWidth;
    int m_windowHeight;

    GLuint m_grassTexture;
    GLuint m_waterTexture;
    GLuint m_heightTexture;

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();


    bool Init();
    bool Shutdown();
    void SetupProjection(int width, int height);
    void Prepare(float dt);
    void Render();
    void DrawTerrain();
};

#endif
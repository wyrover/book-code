#ifndef __GL_COMPONENT
#define __GL_COMPONENT

// extension function and constants
typedef void (APIENTRY * PFNGLFOGCOORDFPROC)(GLfloat);

#define GL_FOG_COORD_SRC    0x8450
#define GL_FOG_COORD        0x8451
#define GL_FRAGMENT_DEPTH   0x8452

const int TERRAIN_SIZE = 65;

class CGfxOpenGL
{
private:
    PFNGLFOGCOORDFPROC myglFogCoordf;
    GLubyte heightmap[TERRAIN_SIZE * TERRAIN_SIZE];
    int m_windowWidth;
    int m_windowHeight;

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();
    void SetupProjection(int width, int height);
    void Prepare(float dt);
    void Render();
    void DrawTerrain();
    void DrawLines();
};

#endif
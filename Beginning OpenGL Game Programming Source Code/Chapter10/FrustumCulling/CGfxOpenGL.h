#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#include "md2.h"
#include "skybox.h"

const int TERRAIN_SIZE = 65;
const float TERRAIN_SCALE = 64.0;
const float ROTATION_SPEED = 40.0f;

const int NUM_CREATURES = 30;

struct plane_t {
    GLfloat A, B, C, D;
};

union frustum_t {
    struct {
        plane_t t, b, l, r, n, f;
    };
    plane_t planes[6];
};

class CGfxOpenGL
{
public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();
    void SetupProjection(int width, int height);
    void Prepare(float dt);
    void Render();

    void BeginRotateRight()
    {
        m_rotateRight = true;
    }
    void BeginRotateLeft()
    {
        m_rotateLeft = true;
    }

    void EndRotateRight()
    {
        m_rotateRight = false;
    }
    void EndRotateLeft()
    {
        m_rotateLeft = false;
    }

    bool ToggleCulling()
    {
        m_useCulling = !m_useCulling;
        return m_useCulling;
    }

private:
    void CalculateFrustum();
    bool InitializeTerrain();
    void DrawTerrain();

    bool InitializeCreatures();
    void DrawCreatures();
    GLfloat GetHeightAt(GLfloat x, GLfloat z);

    int m_windowWidth;
    int m_windowHeight;

    GLuint m_grassTexture;

    GLubyte m_heightmap[TERRAIN_SIZE * TERRAIN_SIZE];


    GLfloat *m_terrain;
    GLfloat *m_terrainUVs;
    GLfloat *m_terrainColors;
    GLuint *m_terrainIndices;

    CMD2Model m_rhinoModel;
    GLfloat m_rhinos[NUM_CREATURES][4];
    CMD2Model m_centaurModel;
    GLfloat m_centaurs[NUM_CREATURES][4];
    CMD2Model m_demonModel;
    GLfloat m_demons[NUM_CREATURES][4];

    GLfloat m_angle;
    bool m_rotateRight;
    bool m_rotateLeft;

    CSkybox m_skybox;

    frustum_t m_frustum;
    bool m_useCulling;
};

#endif
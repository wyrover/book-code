#ifndef __GL_COMPONENT
#define __GL_COMPONENT

const int TERRAIN_SIZE = 260;

#define PI 3.14159f
#define DEG2RAD(x) (x*PI)/180.0f

class CSkybox;

class CGfxOpenGL
{
private:
    GLubyte heightmap[TERRAIN_SIZE * TERRAIN_SIZE];
    int m_windowWidth;
    int m_windowHeight;

    GLuint m_grassTexture;
    GLuint m_waterTexture;

    CSkybox m_skybox;

    float m_angle;
    float m_height;
    float cameraX, cameraY, cameraZ;

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();

    void SetupProjection(int width, int height);

    void Prepare(float dt);
    void Render();

    void MoveCameraRight()
    {
        m_angle += 3.0f;
    }
    void MoveCameraLeft()
    {
        m_angle -= 3.0f;
    }
    void MoveCameraUp()
    {
        m_height += 3.0f;
    }
    void MoveCameraDown()
    {
        m_height -= 3.0f;
    }
    void DrawTerrain();
};

#endif
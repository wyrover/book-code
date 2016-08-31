#ifndef __GL_COMPONENT
#define __GL_COMPONENT

class CGfxOpenGL
{
public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    void SetupProjection(int width, int height);
    bool Shutdown();
    void Prepare(float dt);
    void Render();
    void ToggleFlashlight()
    {
        m_flashlightOn = !m_flashlightOn;
    }

private:
    int m_windowWidth;
    int m_windowHeight;

    float m_cubeAngle;
    float m_lightAngle;
    GLboolean m_flashlightOn;

    GLUquadricObj *m_pSphere;
};

#endif
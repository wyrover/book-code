#ifndef __GL_COMPONENT
#define __GL_COMPONENT

const float DEG_PER_S = 180.0;
const float SPHERE_NUM_SAMPLES = 12;
const float SPHERE_BLUR_ARC = 30.0;

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
    void SetState(int state)
    {
        m_state = state;
    }

    enum {
        MOTION_BLUR,
        SOFT_SHADOW
    };

private:
    void RenderMotionBlur();
    void DrawSpheres(GLfloat angle);
    void RenderSoftShadow();
    void DrawCone();
    void DrawFloor();

    int m_windowWidth;
    int m_windowHeight;

    GLUquadricObj *m_pObj;
    GLfloat m_angle;
    int     m_state;
    GLfloat m_shadowMatrix[16];

};

#endif
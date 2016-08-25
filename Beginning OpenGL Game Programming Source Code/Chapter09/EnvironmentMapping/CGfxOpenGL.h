#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#include "Skybox.h"


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

private:
    void DrawBalls();
    void GenerateEnvTexture();

    int m_windowWidth;
    int m_windowHeight;

    CSkybox m_skybox;

    GLUquadricObj *m_pObj;

    GLuint m_envTexID;
    GLfloat m_angle;
    GLfloat m_ballPos[2][3];
    GLfloat m_ballAngle;
};

#endif
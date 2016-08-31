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

private:
    int m_windowWidth;
    int m_windowHeight;
    GLfloat m_interpol;
    GLuint m_texID[2];
};

#endif
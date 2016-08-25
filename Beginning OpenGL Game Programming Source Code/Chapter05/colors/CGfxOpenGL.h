#ifndef __GL_COMPONENT
#define __GL_COMPONENT

// function pointer type for glSecondaryColor3fEXT
typedef void (APIENTRY * PFNGLSECONDARYCOLOR3FPROC)(GLfloat red, GLfloat green, GLfloat blue);
#define GL_COLOR_SUM    0x8458

class CGfxOpenGL
{
private:
    float vertices[10][3];
    PFNGLSECONDARYCOLOR3FPROC myglSecondaryColor3f;

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();
    void Prepare(float dt);
    void Render();
};

#endif
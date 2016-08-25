#ifndef __GL_COMPONENT
#define __GL_COMPONENT


// extension functions and constants
typedef void (APIENTRY * PFNGLBLENDCOLORPROC)(GLclampf, GLclampf, GLclampf, GLclampf);
typedef void (APIENTRY * PFNGLBLENDEQUATIONPROC)(GLenum);

#define GL_CONSTANT_COLOR             0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR   0x8002
#define GL_CONSTANT_ALPHA             0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA   0x8004
#define GL_FUNC_ADD                   0x8006
#define GL_MIN                        0x8007
#define GL_MAX                        0x8008
#define GL_FUNC_SUBTRACT              0x800A
#define GL_FUNC_REVERSE_SUBTRACT      0x800B

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

    void NextBlendEquation();
    void NextSourceFactor();
    void NextDestFactor();

    const char* GetBlendEquation();
    const char* GetSourceFactor();
    const char* GetDestFactor();

private:
    int m_windowWidth;
    int m_windowHeight;
    GLfloat m_diskPos;
    GLfloat m_diskRot;

    GLUquadricObj *m_pDisk;

    GLenum m_srcFactor;
    GLenum m_dstFactor;
    GLenum m_blendEquation;

    // pointers to extension functions
    PFNGLBLENDEQUATIONPROC myglBlendEquation;
    PFNGLBLENDCOLORPROC myglBlendColor;
};

#endif
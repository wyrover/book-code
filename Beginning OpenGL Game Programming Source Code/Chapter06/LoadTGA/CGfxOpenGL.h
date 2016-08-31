#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#define PI           3.14159
#define TWO_PI       PI*2.0
#define HALF_PI      PI/2.0

class CTargaImage;

class CGfxOpenGL
{
private:
    int m_windowWidth;
    int m_windowHeight;

    CTargaImage *m_tga;
    CTargaImage *m_tgaUncompress;
    unsigned char *m_logoImage24;

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();

    void SetupProjection(int width, int height);

    void Prepare(float dt);
    void Render();
};

#endif
#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#define PI           3.14159
#define TWO_PI       PI*2.0
#define HALF_PI      PI/2.0

class CGfxOpenGL
{
private:
    int m_windowWidth;
    int m_windowHeight;

    float m_angle;

    unsigned int m_fontListBase;
    GLYPHMETRICSFLOAT gmf[256];   // holds orientation and placement
    // info for display lists

    unsigned int CreateOutlineFont(char *fontName, int fontSize, float depth);
    void RenderFont(float xPos, float yPos, float zPos, unsigned int base, char *str);
    void ReleaseFont(unsigned int base);

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
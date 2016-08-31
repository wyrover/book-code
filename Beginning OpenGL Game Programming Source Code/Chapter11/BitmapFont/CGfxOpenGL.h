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

    unsigned int m_fontListBase;

    unsigned int CreateBitmapFont(char *fontName, int fontSize);
    void RenderFont(int xPos, int yPos, unsigned int base, char *str);
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
#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#define PI           3.14159
#define TWO_PI       PI*2.0
#define HALF_PI      PI/2.0

#define MAX_BITMAPS  50

// struct to hold a bitmap raster position
struct BitmapStruct {
    int xPos;
    int yPos;
};

class CGfxOpenGL
{
private:
    int m_windowWidth;
    int m_windowHeight;

public:
    BitmapStruct m_bitmaps[50]; // bitmap positions on the screen

    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();

    void SetupProjection(int width, int height);

    void Prepare(float dt);
    void Render();
};

#endif
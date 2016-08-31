#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#define PI 3.14159
#define TWO_PI PI*2.0
#define HALF_PI PI/2.0

class CGfxOpenGL
{
private:
    int m_windowWidth;
    int m_windowHeight;

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();

    void SetupProjection(int width, int height);

    void DrawCube();
    void PositionAndRotate(float xPos, float yPos, float zPos, float xAngle, float yAngle, float zAngle);

    void Prepare(float dt = 0.0);
    void Render();
};

#endif
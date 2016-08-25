#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#define PI 3.14159
#define TWO_PI PI*2.0
#define HALF_PI PI/2.0

class CGfxOpenGL
{
private:

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();

    void DrawCube(float xPos, float yPos, float zPos);
    void ResizeScene(int width, int height);
    void UpdateProjection(bool toggle = FALSE);

    void Prepare(float dt = 0.0);
    void Render();
};

#endif
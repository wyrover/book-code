#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include "CGfxOpenGL.h"

unsigned char dot[] = {
    0x00, 0x00,
    0x00, 0x00,
    0x0F, 0xF0,
    0x0F, 0xF0,
    0x0F, 0xF0,
    0x0F, 0xF0,
    0x00, 0x00,
    0x00, 0x00
};

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    srand((unsigned int)time(NULL));
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
    if (height == 0) {                  // don't want a divide by zero
        height = 1;
    }

    glViewport(0, 0, width, height);        // reset the viewport to new dimensions
    glMatrixMode(GL_PROJECTION);            // set projection matrix current matrix
    glLoadIdentity();                       // reset projection matrix
    // calculate aspect ratio of window
    glOrtho(0.0f, width - 1.0, 0.0, height - 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
}

void CGfxOpenGL::DrawRandomBitmaps(unsigned char *bitmapData)
{
    for (int idx = 0; idx < 100; idx++) {
        glRasterPos2i(rand() % m_windowWidth, rand() % m_windowHeight);
        glBitmap(8, 8, 0.0, 0.0, 0.0, 0.0, bitmapData);
    }
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // single byte alignment
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // color white
    glColor3f(1.0, 1.0, 1.0);
    DrawRandomBitmaps(dot);
}

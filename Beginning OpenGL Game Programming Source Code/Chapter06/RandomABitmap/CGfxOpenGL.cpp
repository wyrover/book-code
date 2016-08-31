#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include "CGfxOpenGL.h"

// the letter 'A' bitmap
unsigned char letterA[] = {
    0xC0, 0x03,
    0xC0, 0x03,
    0xC0, 0x03,
    0xC0, 0x03,
    0xC0, 0x03,
    0xDF, 0xFB,
    0x7F, 0xFE,
    0x60, 0x06,
    0x30, 0x0C,
    0x30, 0x0C,
    0x18, 0x18,
    0x18, 0x18,
    0x0C, 0x30,
    0x0C, 0x30,
    0x07, 0xE0,
    0x07, 0xE0
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
    // store the random (x, y) position of the bitmaps
    for (int idx = 0; idx < MAX_BITMAPS; idx++) {
        m_bitmaps[idx].xPos = rand() % m_windowWidth;
        m_bitmaps[idx].yPos = rand() % m_windowHeight;
    }
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // single byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // color white
    glColor3f(1.0, 1.0, 1.0);

    // render all the bitmaps
    for (int idx = 0; idx < MAX_BITMAPS; idx++) {
        glRasterPos2i(m_bitmaps[idx].xPos, m_bitmaps[idx].yPos);
        glBitmap(16, 16, 0.0, 0.0, 0.0, 0.0, letterA);
    }
}

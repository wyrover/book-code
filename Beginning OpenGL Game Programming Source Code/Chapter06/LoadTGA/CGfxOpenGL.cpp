#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include "CGfxOpenGL.h"

#include "CTargaImage.h"

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    m_tga = new CTargaImage;
    m_tgaUncompress = new CTargaImage;

    if (!m_tga->Load("opengl_logo.tga"))
        return false;

    if (!m_tgaUncompress->Load("opengl_logo_un.tga"))
        return false;

    return true;
}

bool CGfxOpenGL::Shutdown()
{
    m_tga->Release();
    delete m_tga;
    m_tgaUncompress->Release();
    delete m_tgaUncompress;
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

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // draw compressed TGA at top of window
    glRasterPos2i(250, 400);
    glDrawPixels((int)(m_tga->GetWidth()), (int)(m_tga->GetHeight()),
                 GL_RGB, GL_UNSIGNED_BYTE, m_tga->GetImage());
    // draw uncompressed TGA at bottom of window
    glRasterPos2i(250, 100);
    glDrawPixels((int)(m_tgaUncompress->GetWidth()), (int)(m_tgaUncompress->GetHeight()),
                 GL_RGB, GL_UNSIGNED_BYTE, m_tgaUncompress->GetImage());
}

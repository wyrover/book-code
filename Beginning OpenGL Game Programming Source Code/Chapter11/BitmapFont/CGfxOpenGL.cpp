#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include "CGfxOpenGL.h"

extern HDC hDC;

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    m_fontListBase = CreateBitmapFont("Verdana", 48);
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    ReleaseFont(m_fontListBase);
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
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderFont(270, m_windowHeight / 2, m_fontListBase, "OpenGL Bitmap Fonts!");
}

unsigned int CGfxOpenGL::CreateBitmapFont(char *fontName, int fontSize)
{
    HFONT hFont;     // the windows font
    unsigned int base;
    base = glGenLists(96);

    if (!stricmp(fontName, "symbol")) {
        hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                           SYMBOL_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                           ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
                           fontName);
    } else {
        hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                           ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                           ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
                           fontName);
    }

    if (!hFont)
        return 0;

    SelectObject(hDC, hFont);
    wglUseFontBitmaps(hDC, 32, 96, base);
    return base;
}

void CGfxOpenGL::ReleaseFont(unsigned int base)
{
    if (base != 0)
        glDeleteLists(base, 96);
}

void CGfxOpenGL::RenderFont(int xPos, int yPos, unsigned int base, char *str)
{
    if ((base == 0) || (!str))
        return;

    glRasterPos2i(xPos, yPos);
    glPushAttrib(GL_LIST_BIT);
    glListBase(base - 32);
    glCallLists((int)strlen(str), GL_UNSIGNED_BYTE, str);
    glPopAttrib();
}
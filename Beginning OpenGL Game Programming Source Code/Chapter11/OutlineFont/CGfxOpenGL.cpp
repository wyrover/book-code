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
    glShadeModel(GL_SMOOTH);                     // use smooth shading
    glEnable(GL_DEPTH_TEST);                     // hidden surface removal
    glEnable(GL_LIGHT0);                         // enable light0
    glEnable(GL_LIGHTING);                       // enable lighting
    glEnable(GL_COLOR_MATERIAL);                 // enable color for material
    m_fontListBase = CreateOutlineFont("Verdana", 48, 0.2f);
    m_angle = 0.0f;
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
    // setup perspective
    gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
    m_angle += 15.0f * dt;

    if (m_angle >= 360.0)
        m_angle = 0.0f;
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderFont(0.0, 0.0, -10.0, m_fontListBase, "OpenGL Outline Fonts!");
}

unsigned int CGfxOpenGL::CreateOutlineFont(char *fontName, int fontSize, float depth)
{
    HFONT hFont;     // the windows font
    unsigned int base;
    base = glGenLists(256);

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
    wglUseFontOutlines(hDC, 0, 255, base, 0.0f, depth, WGL_FONT_POLYGONS, gmf);
    return base;
}

void CGfxOpenGL::ReleaseFont(unsigned int base)
{
    if (base != 0)
        glDeleteLists(base, 256);
}

void CGfxOpenGL::RenderFont(float xPos, float yPos, float zPos, unsigned int base, char *str)
{
    float length = 0.0;

    if ((base == 0) || (!str))
        return;

    // center the text
    for (int idx = 0; idx < (int)strlen(str); idx++) {  // find length of text
        length += gmf[str[idx]].gmfCellIncX; // increase length by character's width
    }

    glTranslatef(-length / 2.0f, yPos, zPos);
    glRotatef(m_angle, 1.0, 0.0, 0.0);
    glRotatef(m_angle, 0.0, 1.0, 0.0);
    glRotatef(m_angle, 0.0, 0.0, 1.0);
    glPushAttrib(GL_LIST_BIT);
    glListBase(base);
    glCallLists((int)strlen(str), GL_UNSIGNED_BYTE, str);
    glPopAttrib();
}
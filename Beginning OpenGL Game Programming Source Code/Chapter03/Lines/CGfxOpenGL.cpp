#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "CGfxOpenGL.h"

// disable implicit float-double casting
#pragma warning(disable:4305)

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
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
    gluPerspective(52.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
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
    float lineWidth = 0.5;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen and depth buffer
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    float line;

    // draw a series of lines of increasing width
    for (line = 0.0; line < 7.0; line += 0.5) {
        // set the line width
        glLineWidth(lineWidth);
        // draw the line
        glBegin(GL_LINES);
        glVertex3f(-5.0, 0.0, line - 3.0f);
        glVertex3f(-1.0, 0.0, line - 3.0f);
        glEnd();
        // increase the line width for the next point
        lineWidth += 1.0;
    }

    // reset line width
    lineWidth = 0.5;
    // enable stippling
    glEnable(GL_LINE_STIPPLE);
    // 0xAAAA = 1010 1010 1010 1010
    short stipplePattern = 0xAAAA;
    // set the stipple pattern
    glLineStipple(2, stipplePattern);

    // draw a series of lines of increasing width with stippling
    for (line = 0.0; line < 7.0; line += 0.5) {
        // set the line width
        glLineWidth(lineWidth);
        // draw the point
        glBegin(GL_LINES);
        glVertex3f(1.0f, 0.0f, line - 3.0f);
        glVertex3f(5.0f, 0.0f, line - 3.0f);
        glEnd();
        // increase the point size for the next point
        lineWidth += 1.0;
    }

    glDisable(GL_LINE_STIPPLE);
}


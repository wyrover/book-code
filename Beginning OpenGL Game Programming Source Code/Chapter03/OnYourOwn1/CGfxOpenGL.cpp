#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "CGfxOpenGL.h"

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

void CGfxOpenGL::Prepare(float dt)
{
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

void CGfxOpenGL::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen and depth buffer
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    DrawCircleApproximation(2.0, 10, false);
}

void CGfxOpenGL::DrawCircleApproximation(float radius, int numberOfSides, bool edgeOnly)
{
    // if edge only, use line strips; otherwise, use polygons
    if (edgeOnly)
        glBegin(GL_LINE_STRIP);
    else
        glBegin(GL_POLYGON);

    // calculate each vertex on the circle
    for (int vertex = 0; vertex < numberOfSides; vertex++) {
        // calculate the angle of the current vertex
        float angle = (float)vertex * 2.0f * (float)PI / numberOfSides;
        // draw the current vertex at the correct radius
        glVertex3f(cosf(angle)*radius, 0.0, sinf(angle)*radius);
    }

    // if drawing edge only, then need to complete the loop with first vertex
    if (edgeOnly)
        glVertex3f(radius, 0.0, 0.0);

    glEnd();
}

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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    scaleFactor = 1.0;
    increaseScale = true;
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
    // increase or decrease scale factor
    if (increaseScale)
        scaleFactor += 0.001;
    else
        scaleFactor -= 0.001;

    if (scaleFactor >= 2.0)
        increaseScale = false;
    else if (scaleFactor <= 0.1)
        increaseScale = true;
}

void CGfxOpenGL::DrawPlane()
{
    // draw a red 4x4 unit flat square plane along the x/y axes
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-2.0, 0.0, -2.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(2.0, 0.0, -2.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-2.0, 0.0, 2.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(2.0, 0.0, 2.0);
    glEnd();
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // move eye back 10 units and orient the plane so we can see it
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    // scale the plane along all three axes
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    DrawPlane();
}

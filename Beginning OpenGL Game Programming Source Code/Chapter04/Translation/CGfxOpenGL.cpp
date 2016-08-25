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
    zPos = 0.0;
    direction = true;
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
    // if we're moving in the -z direction, decrement the z position
    if (direction)
        zPos -= 0.01f;
    else    // we're moving in the +z direction, increment the z position
        zPos += 0.01f;

    // if we have rached the origin or -20 units along the
    // z axis, then change direction
    if (zPos >= 0.0)
        direction = true;
    else if (zPos <= -20.0)
        direction = false;
}

void CGfxOpenGL::DrawPlane()
{
    // draw a red 4x4 unit flat square plane along the x/y axes
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-2.0, -2.0, -2.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(2.0, -2.0, -2.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-2.0, -2.0, 2.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(2.0, -2.0, 2.0);
    glEnd();
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // translate the world coordinate system along the z axis
    glTranslatef(0.0, 0.0, zPos);
    // draw the plane at the world origin
    DrawPlane();
}

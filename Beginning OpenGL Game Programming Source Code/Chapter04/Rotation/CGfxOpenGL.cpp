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
    glPolygonMode(GL_BACK, GL_LINE);
    xAxisAngle = 0.0;
    yAxisAngle = 0.0;
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
    // increase rotation angles about each axis
    xAxisAngle += 0.05;
    yAxisAngle += 0.25;

    // if rotation angles hit 360 degrees (complete rotation)
    // then reset the values to 0
    if (xAxisAngle >= 360.0)
        xAxisAngle = 0.0;

    if (yAxisAngle >= 360.0)
        yAxisAngle = 0.0;
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
    // draw plane's local axes
    // y axis from (0,0,0) to (0,3,0)
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 3.0, 0.0);
    glEnd();
    // x axis from (0,0,0) to (3,0,0)
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(3.0, 0.0, 0.0);
    glEnd();
}

void CGfxOpenGL::DrawAxes()
{
    // draw x axis line from (0,0,0) to (3,0,0)
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(3.0, 0.0, 0.0);
    glEnd();
    // draw y axis line from (0,0,0) to (0,3,0)
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 3.0, 0.0);
    glEnd();
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // move eye back 10 units and draw world coordinate x-y axes
    glTranslatef(0.0, 0.0, -10.0);
    DrawAxes();
    // rotate about x axis then y axis at prescribed angles and draw plane
    glRotatef(xAxisAngle, 1.0, 0.0, 0.0);
    glRotatef(yAxisAngle, 0.0, 1.0, 0.0);
    DrawPlane();
}

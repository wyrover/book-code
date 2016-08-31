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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
    static float angle = 0.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen and depth buffer
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPolygonMode(GL_FRONT, GL_LINE);
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, 0.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    Draw2DSquare();
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // reset
    glPolygonMode(GL_BACK, GL_POINT);
    glPushMatrix();
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);
    Draw2DSquare();
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 1.0);
    Draw2DSquare();
    glPopMatrix();
    glPolygonMode(GL_BACK, GL_LINE);
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 0.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1.0, 0.0, 1.0);
    Draw2DSquare();
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 0.0f);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 0.0);
    Draw2DSquare();
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    angle += 0.2f;
}

void CGfxOpenGL::Draw2DSquare()
{
    glBegin(GL_POLYGON);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, -1.0f);
    glVertex3f(-0.5f, 0.0f, -1.0f);
    glEnd();
}


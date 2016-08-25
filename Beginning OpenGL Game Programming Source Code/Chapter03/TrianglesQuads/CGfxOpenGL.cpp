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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear screen and depth buffer
    glLoadIdentity();
    gluLookAt(0.0, 10.0, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // top left
    glPushMatrix();
    glTranslatef(-6.0f, 0.0f, -4.0f);
    DrawPoints();
    glPopMatrix();
    // top middle
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glTranslatef(-2.0f, 0.0f, -4.0f);
    DrawTriangles();
    glPopMatrix();
    // top right
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(2.0f, 0.0f, -4.0f);
    DrawQuads();
    glPopMatrix();
    // bottom left
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(-6.0f, 0.0f, 0.5f);
    DrawTriangleStrip();
    glPopMatrix();
    // bottom middle
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(-2.0f, 0.0f, 0.5f);
    DrawTriangleFan();
    glPopMatrix();
    // bottom right
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(2.0f, 0.0f, 0.5f);
    DrawQuadStrip();
    glPopMatrix();
}

// draw grid of points showing dataset we are working with
void CGfxOpenGL::DrawPoints()
{
    glPointSize(4.0);
    glBegin(GL_POINTS);

    for (int x = 0; x < 4; x++)
        for (int z = 0; z < 4; z++)
            glVertex3f(x, 0, z);

    glEnd();
}

// draw grid as individual triangles
void CGfxOpenGL::DrawTriangles()
{
    glBegin(GL_TRIANGLES);

    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            glVertex3f(x, 0.0, z);
            glVertex3f((x + 1.0), 0.0, z);
            glVertex3f(x, 0.0, (z + 1.0));
        }
    }

    glEnd();
}

// draw grid as triangle fan
void CGfxOpenGL::DrawTriangleFan()
{
    glBegin(GL_TRIANGLE_FAN);
    // center vertex of fan
    glVertex3f(0.0, 0.0, 0.0);

    // bottom side
    for (int x = 4; x > 0; x--)
        glVertex3f(x - 1, 0.0, 3.0);

    // right side
    for (int z = 4; z > 0; z--)
        glVertex3f(3.0, 0.0, z - 1);

    glEnd();
}

// draw grid as triangle strips
void CGfxOpenGL::DrawTriangleStrip()
{
    // 3 rows of triangle strips
    for (int x = 0; x < 3; x++) {
        glBegin(GL_TRIANGLE_STRIP);

        for (int z = 0; z < 3; z++) {
            glVertex3f(x, 0.0, z);
            glVertex3f((x + 1.0), 0.0, z);
            glVertex3f(x, 0.0, (z + 1.0));
            glVertex3f((x + 1.0), 0.0, (z + 1.0));
        }

        glEnd();
    }
}

// draw grid as individual quads
void CGfxOpenGL::DrawQuads()
{
    glBegin(GL_QUADS);

    for (int x = 0; x < 3; x++) {
        for (int z = 0; z < 3; z++) {
            glVertex3f(x, 0.0, z);
            glVertex3f((x + 1.0), 0.0, z);
            glVertex3f((x + 1.0), 0.0, (z + 1.0));
            glVertex3f(x, 0.0, (z + 1.0));
        }
    }

    glEnd();
}

// draw grid as quad strips
void CGfxOpenGL::DrawQuadStrip()
{
    for (int x = 0; x < 3; x++) {
        glBegin(GL_QUAD_STRIP);

        for (int z = 0; z < 4; z++) {
            glVertex3f(x, 0.0, z);
            glVertex3f((x + 1.0), 0.0, z);
        }

        glEnd();
    }
}

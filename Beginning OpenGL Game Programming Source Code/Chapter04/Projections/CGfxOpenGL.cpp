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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    return true;
}

void CGfxOpenGL::Prepare(float dt)
{
}

void CGfxOpenGL::DrawCube(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);   // top face
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   // front face
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   // right face
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);  // left face
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);  // bottom face
    glVertex3f(0.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);  // back face
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

void CGfxOpenGL::ResizeScene(int width, int height)
{
    // avoid divide by zero
    if (height == 0) {
        height = 1;
    }

    // reset the viewport to the new dimensions
    glViewport(0, 0, width, height);
    // set up the projection, without toggling the projection mode
    UpdateProjection(false);
}

void CGfxOpenGL::UpdateProjection(bool toggle)
{
    static bool usePerspective = true;

    // toggle the control variable if appropriate
    if (toggle)
        usePerspective = !usePerspective;

    // select the projection matrix and clear it out
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // choose the appropriate projection based on the currently toggled mode
    if (usePerspective) {
        // set the perspective with the appropriate aspect ratio
        glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    } else {
        // set up an orthographic projection with the same near clip plane
        glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    }

    // select modelview matrix and clear it out
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // position and size the cube
    glTranslatef(0.4, 0.0, -1.5);
    glRotatef(15.0, 1.0, 0.0, 0.0);
    glRotatef(30.0f, 0.0, 1.0, 0.0);
    glScalef(0.75, 0.75, 0.75);
    DrawCube(0.0, 0.0, 0.0);
}

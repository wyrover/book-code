#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "CGfxOpenGL.h"
#include <cmath>

const int SHADOW_NUM_SAMPLES = 13;
const GLfloat LIGHT_POS[][4] = {
    { 3.0f, 6.0f, 2.0f, 1.0f },
    { 3.2f, 6.0f, 2.0f, 1.0f },
    { 2.8f, 6.0f, 2.0f, 1.0f },
    { 3.0f, 6.2f, 2.0f, 1.0f },
    { 3.0f, 5.8f, 2.0f, 1.0f },
    { 3.0f, 6.0f, 2.2f, 1.0f },
    { 3.0f, 6.0f, 1.8f, 1.0f },
    { 3.1f, 6.0f, 2.1f, 1.0f },
    { 2.9f, 6.0f, 2.1f, 1.0f },
    { 3.1f, 6.0f, 1.9f, 1.0f },
    { 2.9f, 6.0f, 1.9f, 1.0f },
    { 3.0f, 6.1f, 2.0f, 1.0f },
    { 3.0f, 5.9f, 2.0f, 1.0f }
};

const GLfloat FLOOR_PLANE[] = { 0.0, 1.0, 0.0, 0.0 };

// create a matrix to project vertices onto a specific plane
void SetShadowMatrix(GLfloat destMat[16], const GLfloat lightPos[4], const GLfloat plane[4])
{
    GLfloat dot;
    // dot product of plane and light position
    dot = plane[0] * lightPos[0] + plane[1] * lightPos[1] + plane[2] * lightPos[2] + plane[3] * lightPos[3];
    // first column
    destMat[0] = dot - lightPos[0] * plane[0];
    destMat[4] = 0.0f - lightPos[0] * plane[1];
    destMat[8] = 0.0f - lightPos[0] * plane[2];
    destMat[12] = 0.0f - lightPos[0] * plane[3];
    // second column
    destMat[1] = 0.0f - lightPos[1] * plane[0];
    destMat[5] = dot - lightPos[1] * plane[1];
    destMat[9] = 0.0f - lightPos[1] * plane[2];
    destMat[13] = 0.0f - lightPos[1] * plane[3];
    // third column
    destMat[2] = 0.0f - lightPos[2] * plane[0];
    destMat[6] = 0.0f - lightPos[2] * plane[1];
    destMat[10] = dot - lightPos[2] * plane[2];
    destMat[14] = 0.0f - lightPos[2] * plane[3];
    // fourth column
    destMat[3] = 0.0f - lightPos[3] * plane[0];
    destMat[7] = 0.0f - lightPos[3] * plane[1];
    destMat[11] = 0.0f - lightPos[3] * plane[2];
    destMat[15] = dot - lightPos[3] * plane[3];
}

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    m_angle = 0.0;
    m_state = MOTION_BLUR;
    m_pObj = gluNewQuadric();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
    if (height == 0) {                  // don't want a divide by zero
        height = 1;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
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

bool CGfxOpenGL::Shutdown()
{
    gluDeleteQuadric(m_pObj);
    return true;
}


void CGfxOpenGL::Prepare(float dt)
{
    m_angle += dt * DEG_PER_S;

    if (m_angle > 360.0)
        m_angle = 0.0;
}

void CGfxOpenGL::Render()
{
    switch (m_state) {
    case MOTION_BLUR:
        RenderMotionBlur();
        break;

    case SOFT_SHADOW:
        RenderSoftShadow();
        break;

    default:
        break;
    }
}

void CGfxOpenGL::RenderMotionBlur()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    GLfloat angle = m_angle;

    for (int i = 0; i < SPHERE_NUM_SAMPLES; ++i) {
        DrawSpheres(angle);
        angle -= SPHERE_BLUR_ARC / SPHERE_NUM_SAMPLES;

        if (i == 0)
            glAccum(GL_LOAD, 1.0f / SPHERE_NUM_SAMPLES);
        else
            glAccum(GL_ACCUM, 1.0f / SPHERE_NUM_SAMPLES);
    }

    glAccum(GL_RETURN, 1.0);
}

void CGfxOpenGL::DrawSpheres(GLfloat angle)
{
    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, -1.0f);
    glPushMatrix();
    glColor3f(0.0f, 0.8f, 0.4f);
    glTranslatef(-2.5f, 0.0f, 0.0f);
    gluSphere(m_pObj, 1.5f, 32, 32);
    glPopMatrix();
    glColor3f(1.0f, 0.8f, 0.1f);
    glTranslatef(2.5f, 0.0f, 0.0f);
    gluSphere(m_pObj, 1.5f, 32, 32);
    glPopMatrix();
}


void CGfxOpenGL::RenderSoftShadow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5.0, 8.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);

    for (int i = 0; i < SHADOW_NUM_SAMPLES; ++i) {
        SetShadowMatrix(m_shadowMatrix, LIGHT_POS[i], FLOOR_PLANE);
        // draw the shadow
        glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS[0]);
        DrawFloor();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glColor4f(0.0, 0.0, 0.0, 1.0f);
        // project the cone through the shadow matrix
        glMultMatrixf(m_shadowMatrix);
        DrawCone();
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        glAccum(GL_ACCUM, 1.0f / SHADOW_NUM_SAMPLES);
    }

    glAccum(GL_RETURN, 1.0);
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS[0]);
    // draw the cone normally
    glEnable(GL_LIGHTING);
    glColor3f(0.1f, 0.2f, 0.8f);
    DrawCone();
    glDisable(GL_LIGHTING);
}


void CGfxOpenGL::DrawCone()
{
    glPushMatrix();
    glTranslatef(0.0, 3.0, 1.0);
    glRotatef(m_angle, 1.0f, 0.1f, 0.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(m_pObj, 0, 1.0, 2.0, 32, 8);
    glPopMatrix();
}

void CGfxOpenGL::DrawFloor()
{
    glColor3f(1.0, 1.0, 1.0);
    glNormal3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-4.0, 0.0, 4.0);
    glVertex3f(4.0, 0.0, 4.0);
    glVertex3f(4.0, 0.0, -4.0);
    glVertex3f(-4.0, 0.0, -4.0);
    glEnd();
}

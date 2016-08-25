#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "CGfxOpenGL.h"
#include <cmath>

const float CUBE_DEG_PER_S = 30.0f;
const float LIGHT_DEG_PER_S = 90.0f;

const float CUBE_SIZE = 9.0f;

const GLfloat flashLightPos[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat flashLightDir[] = { 0.0, 0.0, -1.0 };
const GLfloat flashLightColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };

const GLfloat redLightColor[] = { 0.5f, 0.1f, 0.2f, 1.0f };
const GLfloat redLightPos[] = { 10.0, 0.0, 5.0, 1.0 };

const GLfloat greenLightColor[] = { 0.1f, 0.6f, 0.2f, 1.0f };
const GLfloat greenLightPos[] = { 0.0, 0.0, 10.0, 1.0 };

// draw one side of the cube
void DrawPlane(float size, float step)
{
    glBegin(GL_QUADS);

    for (float z = 0.0; z < size; z += step) {
        for (float x = 0.0; x < size; x += step) {
            glVertex3f(x, 0.0, z);
            glVertex3f(x + step, 0.0, z);
            glVertex3f(x + step, 0.0, z + step);
            glVertex3f(x, 0.0, z + step);
        }
    }

    glEnd();
}

// draw the entire cube
void DrawCube(float size, int resolution)
{
    float step = size / resolution;
    glPushMatrix();
    glTranslatef(-size / 2, -size / 2, -size / 2);
    glNormal3f(0.0, -1.0, 0.0);
    // top
    glPushMatrix();
    glTranslatef(0.0, size, 0.0);
    glScalef(1.0, -1.0, 1.0);
    DrawPlane(size, step);
    glPopMatrix();
    // bottom
    DrawPlane(size, step);
    // left
    glPushMatrix();
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glScalef(1.0, -1.0, 1.0);
    DrawPlane(size, step);
    glPopMatrix();
    // right
    glPushMatrix();
    glTranslatef(size, 0.0, 0.0);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    DrawPlane(size, step);
    glPopMatrix();
    // front
    glPushMatrix();
    glTranslatef(0.0, 0.0, size);
    glRotatef(90.0, -1.0, 0.0, 0.0);
    DrawPlane(size, step);
    glPopMatrix();
    // back
    glPushMatrix();
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glScalef(1.0, -1.0, 1.0);
    DrawPlane(size, step);
    glPopMatrix();
    glPopMatrix();
}


CGfxOpenGL::CGfxOpenGL()
{
    m_cubeAngle = 0.0;
    m_lightAngle = 0.0;
    m_pSphere = NULL;
    m_flashlightOn = GL_FALSE;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    // set up the flashlight
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, flashLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, flashLightColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, flashLightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, flashLightColor);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, flashLightDir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 12.0);
    // set up static red light
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, redLightColor);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, redLightColor);
    glLightfv(GL_LIGHT1, GL_SPECULAR, redLightColor);
    // set up moving green light
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, greenLightColor);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, greenLightColor);
    glLightfv(GL_LIGHT2, GL_SPECULAR, greenLightColor);
    // get a quadric object for the light sphere
    m_pSphere = gluNewQuadric();
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
    gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

bool CGfxOpenGL::Shutdown()
{
    if (m_pSphere)
        gluDeleteQuadric(m_pSphere);

    m_pSphere = NULL;
    return true;
}


void CGfxOpenGL::Prepare(float dt)
{
    m_cubeAngle += CUBE_DEG_PER_S * dt;

    if (m_cubeAngle > 360.0)
        m_cubeAngle = 0.0;

    m_lightAngle += LIGHT_DEG_PER_S * dt;

    if (m_lightAngle > 360.0)
        m_lightAngle = 0.0;
}

void CGfxOpenGL::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if (m_flashlightOn)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);

    // position the red light
    glLightfv(GL_LIGHT1, GL_POSITION, redLightPos);
    // draw the red light
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(redLightPos[0], redLightPos[1], redLightPos[2]);
    glColor3fv(redLightColor);
    gluSphere(m_pSphere, 0.2f, 10, 10);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    // position and draw the green light
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glRotatef(m_lightAngle, 1.0, 0.0, 0.0);
    glRotatef(m_lightAngle, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT2, GL_POSITION, greenLightPos);
    glTranslatef(greenLightPos[0], greenLightPos[1], greenLightPos[2]);
    glColor3fv(greenLightColor);
    gluSphere(m_pSphere, 0.2f, 10, 10);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    // set up cube's material
    GLfloat cubeColor[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    GLfloat cubeSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cubeColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cubeSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0f);
    // position and draw the cube
    glPushMatrix();
    glRotatef(m_cubeAngle, 1.0, 1.0, 0.0);
    glRotatef(m_cubeAngle, 0.0, 0.0, 1.0);
    DrawCube(CUBE_SIZE, 256);
    glPopMatrix();
}



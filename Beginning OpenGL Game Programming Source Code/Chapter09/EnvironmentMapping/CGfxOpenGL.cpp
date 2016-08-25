#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"
#include "CGfxOpenGL.h"
#include "CTargaImage.h"
#include <cmath>


const GLfloat DEG_PER_S = 90.0f;
const GLfloat BALL_DEG_PER_S = 3.0f;

const GLuint ENV_TEX_SIZE = 256;

// used to set up the six camera angles
const GLfloat ENV_ROTATION[6][6] = {
    // at              up
    { 1.0, 0.0, 0.0,   0.0, -1.0, 0.0 },  // pos x
    { -1.0, 0.0, 0.0,  0.0, -1.0, 0.0 },  // neg x
    { 0.0, 1.0, 0.0,   0.0, 0.0, 1.0 },   // pos y
    { 0.0, -1.0, 0.0,  0.0, 0.0, -1.0 },  // neg y
    { 0.0, 0.0, 1.0,   0.0, -1.0, 0.0 },  // poz z
    { 0.0, 0.0, -1.0,  0.0, -1.0, 0.0 }   // neg z
};

CGfxOpenGL::CGfxOpenGL()
{
    m_pObj = NULL;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    m_skybox.Initialize(5.0);
    m_skybox.LoadTextures("skybox/up.tga", "skybox/dn.tga", "skybox/ft.tga",
                          "skybox/bk.tga", "skybox/lf.tga", "skybox/rt.tga");
    m_pObj = gluNewQuadric();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    // set up lighting
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
    glGenTextures(1, &m_envTexID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // set the texture coordinate generation mode to reflection mapping
    glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
    m_angle = 0.0;
    m_ballAngle = 0.0;
    return true;
}

void CGfxOpenGL::SetupProjection(int width, int height)
{
    if (height == 0) {                // don't want a divide by zero
        height = 1;
    }

    glViewport(0, 0, width, height);      // reset the viewport to new dimensions
    glMatrixMode(GL_PROJECTION);          // set projection matrix current matrix
    glLoadIdentity();                     // reset projection matrix
    // calculate aspect ratio of window
    gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);               // set modelview matrix
    glLoadIdentity();                     // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

bool CGfxOpenGL::Shutdown()
{
    m_skybox.Release();
    gluDeleteQuadric(m_pObj);
    glDeleteTextures(1, &m_envTexID);
    return true;
}


void CGfxOpenGL::Prepare(float dt)
{
    m_angle += dt * DEG_PER_S;

    if (m_angle > 360.0f)
        m_angle = 0.0f;

    m_ballAngle += dt * BALL_DEG_PER_S;
    // rotate the balls
    m_ballPos[0][0] = sinf(m_ballAngle) * 2.0f;
    m_ballPos[0][1] = cosf(m_ballAngle) * 2.0f;
    m_ballPos[0][2] = sinf(m_ballAngle / 2.0f) * 2.0f;
    m_ballPos[1][0] = sinf(-m_ballAngle / 2.0f) * 2.0f;
    m_ballPos[1][1] = cosf(-m_ballAngle / 2.0f) * 2.0f;
    m_ballPos[1][2] = sinf(-m_ballAngle / 4.0f) * 2.0f;
}

void CGfxOpenGL::Render()
{
    // update the environment map
    GenerateEnvTexture();
    // clear the depth buffer only. no need to clear the color buffer because
    // the skybox will fill the screen
    glClear(GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    m_skybox.Render(0.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -5.0);
    // draw the reflective sphere with texturing and lighting
    GLfloat lightPos[] = { 0.5f, 0.5, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    // enable cube mpapping
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(m_pObj, 1.0, 64, 64);
    glDisable(GL_TEXTURE_CUBE_MAP);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    // draw the rotating balls
    DrawBalls();
}


void CGfxOpenGL::DrawBalls()
{
    GLfloat lightPos[] = { 0.5f, 0.5, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    // draw the green ball
    glPushMatrix();
    glColor3f(0.2f, 0.5f, 0.2f);
    glTranslatef(m_ballPos[0][0], m_ballPos[0][1], m_ballPos[0][2]);
    gluSphere(m_pObj, 0.2f, 16, 16);
    glPopMatrix();
    // draw the yellow ball
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.3f);
    glTranslatef(m_ballPos[1][0], m_ballPos[1][1], m_ballPos[1][2]);
    gluSphere(m_pObj, 0.3f, 16, 16);
    glPopMatrix();
}


void CGfxOpenGL::GenerateEnvTexture()
{
    static bool s_initialized = false;
    // set the screen to match the enviroment map's size
    glViewport(0, 0, ENV_TEX_SIZE, ENV_TEX_SIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // set a 90 degree FOV
    gluPerspective(90, 1, 0.1, 500);
    glMatrixMode(GL_MODELVIEW);
    // bind the cube map texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envTexID);

    // cycle through all six viewing directions
    for (int i = 0; i < 6; ++i) {
        glClear(GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 0.0,
                  ENV_ROTATION[i][0], ENV_ROTATION[i][1], ENV_ROTATION[i][2],
                  ENV_ROTATION[i][3], ENV_ROTATION[i][4], ENV_ROTATION[i][5]);
        // draw the reflected objects
        m_skybox.Render(0.0, 0.0, 0.0);
        DrawBalls();

        // update the texture rather than creating a new one if this isn't the first pass
        // texture is copied from the color buffer
        if (s_initialized) {
            glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, 0, 0, ENV_TEX_SIZE, ENV_TEX_SIZE);
        } else {
            glCopyTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 0, 0, ENV_TEX_SIZE, ENV_TEX_SIZE, 0);
        }
    }

    s_initialized = true;
    // restore the original projection and viewport
    SetupProjection(m_windowWidth, m_windowHeight);
}

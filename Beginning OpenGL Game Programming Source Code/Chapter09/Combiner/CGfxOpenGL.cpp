#ifdef _WINDOWS
    #include <windows.h>
#endif

#include "glee.h"
#include <gl\glu.h>
#include "CGfxOpenGL.h"
#include "CTargaImage.h"

const float TOTAL_TIME = 5.0f;

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    if (!GLeeInit())
        return false;

    if (!GLEE_VERSION_1_3)
        return false;

    CTargaImage image;
    glGenTextures(2, m_texID);
    // load one texture into texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID[0]);
    image.Load("2.tga");
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    glEnable(GL_TEXTURE_2D);
    // pass the texture through to the next unit
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // load the other into texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texID[1]);
    image.Load("1.tga");
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    glEnable(GL_TEXTURE_2D);
    // set the combine mode
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    // use the interpolate combiner function
    glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
    // set Arg0 to be the output of texture unit 0
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE0);
    // set Arg1 to be the current texture image
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
    // set Arg2 to be the texture env color for tex unit 1
    glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_CONSTANT);
    // use the constant alpha to modify the rgb components
    glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
    m_interpol = 0.0;
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

bool CGfxOpenGL::Shutdown()
{
    return true;
}


void CGfxOpenGL::Prepare(float dt)
{
    static float scale = 1.0;
    m_interpol += scale * dt / TOTAL_TIME;

    if (m_interpol > 1.0) {
        m_interpol = 1.0;
        scale = -1.0;
    } else if (m_interpol < 0.0) {
        m_interpol = 0.0;
        scale = 1.0;
    }
}

void CGfxOpenGL::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat texEnvColor[] = { 0.0, 0.0, 0.0, 0.0 };
    texEnvColor[3] = m_interpol;
    glActiveTexture(GL_TEXTURE1);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texEnvColor);
    // draw a single quad with texture coordinates for each texture unit
    glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
    glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
    glVertex3f(-1.0, -1.0f, -2.0);
    glMultiTexCoord2f(GL_TEXTURE0, 1.0, 0.0);
    glMultiTexCoord2f(GL_TEXTURE1, 1.0, 0.0);
    glVertex3f(1.0, -1.0f, -2.0);
    glMultiTexCoord2f(GL_TEXTURE0, 1.0, 1.0);
    glMultiTexCoord2f(GL_TEXTURE1, 1.0, 1.0);
    glVertex3f(1.0, 1.0f, -2.0);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0, 1.0);
    glMultiTexCoord2f(GL_TEXTURE1, 0.0, 1.0);
    glVertex3f(-1.0, 1.0f, -2.0);
    glEnd();
}



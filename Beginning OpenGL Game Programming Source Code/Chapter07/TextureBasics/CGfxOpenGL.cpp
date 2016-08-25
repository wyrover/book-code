#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include "glext.h"
#include "CGfxOpenGL.h"

#include "CTargaImage.h"

CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // enable 2D texturing
    glEnable(GL_TEXTURE_2D);
    m_textureOne = new CTargaImage;

    // load texture image data
    if (!m_textureOne->Load("rock.tga"))
        return false;

    // retrieve "unused" texture object
    glGenTextures(1, &m_textureObjectOne);
    // bind the texture object
    glBindTexture(GL_TEXTURE_2D, m_textureObjectOne);
    // minimum required to set the min and mag texture filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // now that the texture object is bound, specify a texture for it
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureOne->GetWidth(), m_textureOne->GetHeight(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureOne->GetImage());
    // create the second texture object
    glGenTextures(1, &m_textureObjectTwo);
    glBindTexture(GL_TEXTURE_2D, m_textureObjectTwo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_textureOne->GetWidth(), m_textureOne->GetHeight(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, m_textureOne->GetImage());
    // initialize movement variables
    m_zPos = -5.0f;
    m_zMoveNegative = true;
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    glDeleteTextures(1, &m_textureObjectOne);
    glDeleteTextures(1, &m_textureObjectTwo);
    m_textureOne->Release();
    delete m_textureOne;
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
    // calculate perspective
    gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
    // change polygon z position
    if (m_zMoveNegative)
        m_zPos -= 5.0f * dt;
    else
        m_zPos += 5.0f * dt;

    if (m_zPos > -5.0f) {
        m_zPos = -5.0f;
        m_zMoveNegative = true;
    }

    if (m_zPos < -20.0f) {
        m_zPos = -20.0f;
        m_zMoveNegative = false;
    }
}

void CGfxOpenGL::DrawPlane()
{
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(2.0, 0.0);
    glVertex3f(2.0, -2.0, -2.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-2.0, -2.0, -2.0);
    glTexCoord2f(2.0, 2.0);
    glVertex3f(2.0, -2.0, 2.0);
    glTexCoord2f(0.0, 2.0);
    glVertex3f(-2.0, -2.0, 2.0);
    glEnd();
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // load the identity matrix (clear to default position and orientation)
    glLoadIdentity();
    // draw the left polygon
    glPushMatrix();
    // translate the world coordinate system along the z axis
    glTranslatef(-3.0, 0.0, m_zPos);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    // bind the texture
    glBindTexture(GL_TEXTURE_2D, m_textureObjectOne);
    // draw the plane at the world origin
    DrawPlane();
    glPopMatrix();
    // do it all again for the right polygon
    glPushMatrix();
    glTranslatef(3.0, 0.0, m_zPos);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, m_textureObjectTwo);
    DrawPlane();
    glPopMatrix();
}

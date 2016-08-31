#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"

#include "CTargaImage.h"
#include "Skybox.h"


CSkybox::CSkybox()
{
    for (int idx = 0; idx < 6; idx++)
        m_textures[idx] = 0;
}


CSkybox::~CSkybox()
{
    Release();
}


void CSkybox::Initialize(float size)
{
    m_size = size;
}


bool CSkybox::LoadTextures(char* top, char* bottom, char* front, char* back, char* left, char* right)
{
    CTargaImage image;
    image.Load(top);
    glGenTextures(1, &m_textures[SKY_TOP]);
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_TOP]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    image.Load(bottom);
    glGenTextures(1, &m_textures[SKY_BOTTOM]);
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_BOTTOM]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    image.Load(front);
    glGenTextures(1, &m_textures[SKY_FRONT]);
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_FRONT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    image.Load(back);
    glGenTextures(1, &m_textures[SKY_BACK]);
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_BACK]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    image.Load(left);
    glGenTextures(1, &m_textures[SKY_LEFT]);
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_LEFT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    image.Load(right);
    glGenTextures(1, &m_textures[SKY_RIGHT]);
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_RIGHT]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    return true;
}


void CSkybox::Render(float cameraX, float cameraY, float cameraZ)
{
    glPushMatrix();
    // Move the skybox so that it's centered on the camera.
    glTranslatef(cameraX, cameraY, cameraZ);
    glPushAttrib(GL_FOG_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
    glDisable(GL_DEPTH_TEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // Top
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_TOP]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-m_size, m_size, -m_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(m_size, m_size, -m_size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(m_size, m_size, m_size);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-m_size, m_size, m_size);
    glEnd();
    // Bottom
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_BOTTOM]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_size, -m_size, -m_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-m_size, -m_size, -m_size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-m_size, -m_size, m_size);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(m_size, -m_size, m_size);
    glEnd();
    // Front
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_FRONT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-m_size, -m_size, -m_size);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(m_size, -m_size, -m_size);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_size, m_size, -m_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-m_size, m_size, -m_size);
    glEnd();
    // Back
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_BACK]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(m_size, -m_size, m_size);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-m_size, -m_size, m_size);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-m_size, m_size, m_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(m_size, m_size, m_size);
    glEnd();
    // Right
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_RIGHT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(m_size, -m_size, m_size);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_size, m_size, m_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(m_size, m_size, -m_size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(m_size, -m_size, -m_size);
    glEnd();
    // Left
    glBindTexture(GL_TEXTURE_2D, m_textures[SKY_LEFT]);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-m_size, -m_size, -m_size);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-m_size, m_size, -m_size);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-m_size, m_size, m_size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-m_size, -m_size, m_size);
    glEnd();
    glPopAttrib();
    glEndList();
    glPopMatrix();
}


void CSkybox::Release()
{
    for (int i = 0; i < 6; ++i)
        glDeleteTextures(6, &m_textures[0]);
}
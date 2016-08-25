#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <time.h>
#include <cmath>
#include <cstdio>
#include "glext.h"

#include "Skybox.h"
#include "CGfxOpenGL.h"

#include "CTargaImage.h"

#pragma warning(disable:4244)

const char heightmapFilename[] = "heightmap.raw";

const float MAX_HEIGHT = 30.0f;
const float MAX_FOG_HEIGHT = MAX_HEIGHT * 0.5f;
const float SCALE_FACTOR = 256.0f / MAX_HEIGHT;
const float WATER_HEIGHT = 2.0f;

CGfxOpenGL::CGfxOpenGL()
{
    m_grassTexture = m_waterTexture = 0;
    cameraX = cameraY = cameraZ = 0.0f;
    m_angle = 0.0;
    m_height = 10.0;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    FILE *pFile = fopen(heightmapFilename, "rb");

    if (!pFile)
        return false;

    fread(&heightmap, TERRAIN_SIZE * TERRAIN_SIZE, 1, pFile);
    fclose(pFile);
    CTargaImage image;
    image.Load("grass.tga");
    glGenTextures(1, &m_grassTexture);
    glBindTexture(GL_TEXTURE_2D, m_grassTexture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    image.Load("water.tga");
    glGenTextures(1, &m_waterTexture);
    glBindTexture(GL_TEXTURE_2D, m_waterTexture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    m_skybox.Initialize(10.0);
    m_skybox.LoadTextures("up.tga", "dn.tga", "ft.tga", "bk.tga", "lt.tga", "rt.tga");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    glDeleteTextures(1, &m_grassTexture);
    glDeleteTextures(1, &m_waterTexture);
    m_skybox.Release();
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
    cameraX = sin(DEG2RAD(m_angle)) * TERRAIN_SIZE / 2.0;
    cameraY = m_height;
    cameraZ = cos(DEG2RAD(m_angle)) * TERRAIN_SIZE / 2.0;
}


void CGfxOpenGL::DrawTerrain()
{
    // draw the terrain
    glBindTexture(GL_TEXTURE_2D, m_grassTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for (int z = 0; z < TERRAIN_SIZE - 1; ++z) {
        glBegin(GL_TRIANGLE_STRIP);

        for (int x = 0; x < TERRAIN_SIZE; ++x) {
            // render two vertices of the strip at once
            float scaledHeight = heightmap[z * TERRAIN_SIZE + x] / SCALE_FACTOR;
            float nextScaledHeight = heightmap[(z + 1) * TERRAIN_SIZE + x] / SCALE_FACTOR;
            float color = 0.5f + 0.5f * scaledHeight / MAX_HEIGHT;
            float nextColor = 0.5f + 0.5f * nextScaledHeight / MAX_HEIGHT;
            glColor3f(color, color, color);
            glTexCoord2f((GLfloat)x / TERRAIN_SIZE * 8, (GLfloat)z / TERRAIN_SIZE * 8);
            glVertex3f(static_cast<GLfloat>(x - TERRAIN_SIZE / 2), scaledHeight, static_cast<GLfloat>(z - TERRAIN_SIZE / 2));
            glColor3f(nextColor, nextColor, nextColor);
            glTexCoord2f((GLfloat)x / TERRAIN_SIZE * 8, (GLfloat)(z + 1) / TERRAIN_SIZE * 8);
            glVertex3f(static_cast<GLfloat>(x - TERRAIN_SIZE / 2), nextScaledHeight, static_cast<GLfloat>(z + 1 - TERRAIN_SIZE / 2));
        }

        glEnd();
    }

    //draw the water
    glBindTexture(GL_TEXTURE_2D, m_waterTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-TERRAIN_SIZE / 2.1f, WATER_HEIGHT, TERRAIN_SIZE / 2.1f);
    glTexCoord2f(TERRAIN_SIZE / 4.0f, 0.0);
    glVertex3f(TERRAIN_SIZE / 2.1f, WATER_HEIGHT, TERRAIN_SIZE / 2.1f);
    glTexCoord2f(TERRAIN_SIZE / 4.0f, TERRAIN_SIZE / 4.0f);
    glVertex3f(TERRAIN_SIZE / 2.1f, WATER_HEIGHT, -TERRAIN_SIZE / 2.1f);
    glTexCoord2f(0.0, TERRAIN_SIZE / 4.0f);
    glVertex3f(-TERRAIN_SIZE / 2.1f, WATER_HEIGHT, -TERRAIN_SIZE / 2.1f);
    glEnd();
}

void CGfxOpenGL::Render()
{
    glClearColor(0.0f, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // skybox origin should be same as camera position
    m_skybox.Render(cameraX, cameraY, cameraZ);
    DrawTerrain();
}

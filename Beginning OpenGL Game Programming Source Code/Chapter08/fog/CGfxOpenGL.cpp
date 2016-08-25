#ifdef _WINDOWS
    #include <windows.h>
#endif

#include "CGfxOpenGL.h"
#include "CTargaImage.h"

#include <gl/glu.h>
#include <cmath>
#include <cstdio>

const char heightmapFilename[] = "heightmap.raw";

const float MAX_HEIGHT = 10.0f;
const float MAX_FOG_HEIGHT = MAX_HEIGHT * 0.5f;
const float SCALE_FACTOR = 256.0f / MAX_HEIGHT;
const float WATER_HEIGHT = 0.8f;

CGfxOpenGL::CGfxOpenGL()
{
    m_grassTexture = m_waterTexture = 0;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    if (!GLeeInit())
        return false;

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
    glLineWidth(3.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    glDeleteTextures(1, &m_grassTexture);
    glDeleteTextures(1, &m_waterTexture);
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
    glClearColor(0.7f, 0.7f, 0.9f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set up the view
    glLoadIdentity();
    gluLookAt(TERRAIN_SIZE / 3.0f, MAX_HEIGHT + 1.0, TERRAIN_SIZE / 3.0f, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // use grey for all fog
    GLfloat fogColor[] = { 0.5f, 0.5f, 0.5f };
    glFogfv(GL_FOG_COLOR, fogColor);
    // without fog
    glViewport(0, m_windowHeight / 2, m_windowWidth / 2, m_windowHeight / 2);
    DrawTerrain();
    // with linear fog
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 10.0);
    glFogf(GL_FOG_END, 60.0);
    glViewport(m_windowWidth / 2, m_windowHeight / 2, m_windowWidth / 2, m_windowHeight / 2);
    DrawTerrain();
    // with exponential fog
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.03f);
    glViewport(0, 0, m_windowWidth / 2, m_windowHeight / 2);
    DrawTerrain();

    if (GLEE_EXT_fog_coord) {
        // with fog coords
        glFogi(GL_FOG_COORD_SOURCE, GL_FOG_COORD);
        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogf(GL_FOG_DENSITY, 1.2f);
        glViewport(m_windowWidth / 2, 0, m_windowWidth / 2, m_windowHeight / 2);
        DrawTerrain();
        // switch back to defaults
        glFogi(GL_FOG_COORD_SOURCE, GL_FRAGMENT_DEPTH);
    }

    glDisable(GL_FOG);
    DrawLines();
}

float ComputeFogCoord(float height)
{
    if (height > MAX_FOG_HEIGHT)
        height = MAX_FOG_HEIGHT;
    else if (height < WATER_HEIGHT)
        height = WATER_HEIGHT;

    // Height is now between WATER_HEIGHT and MAX_FOG_HEIGHT. Scale
    // it so that the fog coord is 0.0 at the maximum height and 1.0 at the
    // water level
    height = height - WATER_HEIGHT;
    height = 1.0f - (height / (MAX_FOG_HEIGHT - WATER_HEIGHT));
    return height;
}

void CGfxOpenGL::DrawTerrain()
{
    glBindTexture(GL_TEXTURE_2D, m_grassTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for (int z = 0; z < TERRAIN_SIZE - 1; ++z) {
        glBegin(GL_TRIANGLE_STRIP);

        for (int x = 0; x < TERRAIN_SIZE; ++x) {
            float scaledHeight = heightmap[z * TERRAIN_SIZE + x] / SCALE_FACTOR;
            float nextScaledHeight = heightmap[(z + 1) * TERRAIN_SIZE + x] / SCALE_FACTOR;
            float color = 0.5f + 0.5f * scaledHeight / MAX_HEIGHT;
            float nextColor = 0.5f + 0.5f * nextScaledHeight / MAX_HEIGHT;

            if (GLEE_VERSION_1_4)
                glFogCoordf(ComputeFogCoord(scaledHeight));
            else if (GLEE_EXT_fog_coord)
                glFogCoordfEXT(ComputeFogCoord(scaledHeight));

            glColor3f(color, color, color);
            glTexCoord2f((GLfloat)x / TERRAIN_SIZE * 4, (GLfloat)z / TERRAIN_SIZE * 4);
            glVertex3f(static_cast<GLfloat>(x - TERRAIN_SIZE / 2), scaledHeight, static_cast<GLfloat>(z - TERRAIN_SIZE / 2));

            if (GLEE_VERSION_1_4)
                glFogCoordf(ComputeFogCoord(nextScaledHeight));
            else if (GLEE_EXT_fog_coord)
                glFogCoordfEXT(ComputeFogCoord(nextScaledHeight));

            glColor3f(nextColor, nextColor, nextColor);
            glTexCoord2f((GLfloat)x / TERRAIN_SIZE * 4, (GLfloat)(z + 1) / TERRAIN_SIZE * 4);
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


void CGfxOpenGL::DrawLines()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 2.0, 0.0, 2.0);
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0, 1.0, 0.5f);
    glBegin(GL_LINES);
    glVertex2i(2, 1);
    glVertex2i(0, 1);
    glVertex2i(1, 2);
    glVertex2i(1, 0);
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}
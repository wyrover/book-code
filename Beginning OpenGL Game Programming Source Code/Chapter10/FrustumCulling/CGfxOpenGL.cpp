#ifdef _WINDOWS
    #include <windows.h>
#endif

#include "glee.h"
#include <cmath>
#include "CGfxOpenGL.h"
#include <cstdio>


extern long windowHeight;
extern long windowWidth;

const char heightmapFilename[] = "heightmap.raw";


const float EYE_HEIGHT = 40.0f;
const float TERRAIN_HEIGHT_SCALE = 0.4f;
const float TERRAIN_MAX_HEIGHT = 256 * TERRAIN_HEIGHT_SCALE;
const float CREATURE_MIN_DISTANCE = 140.0f;
const float CREATURE_SCALE = TERRAIN_SCALE * 0.9f;

// returns a number ranging from -1.0 to 1.0
#define FRAND()   (((float)rand()-(float)rand())/RAND_MAX)


bool SphereInFrustum(sphere_t sphere, frustum_t frustum)
{
    GLfloat dist;

    for (int i = 0; i < 6; ++i) {
        dist = frustum.planes[i].A * sphere.center.x +
               frustum.planes[i].B * sphere.center.y +
               frustum.planes[i].C * sphere.center.z +
               frustum.planes[i].D;

        if (dist <= -sphere.radius)
            return false;
    }

    return true;
}


CGfxOpenGL::CGfxOpenGL()
{
    m_terrain = NULL;
    m_terrainUVs = NULL;
    m_terrainIndices = NULL;
    m_terrainColors = NULL;
    m_angle = 0.0;
    m_rotateRight = false;
    m_rotateLeft = false;
    m_useCulling = true;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    GLeeInit();
    glLineWidth(3.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    InitializeTerrain();
    InitializeCreatures();
    CTargaImage image;
    image.Load("grass.tga");
    glGenTextures(1, &m_grassTexture);
    glBindTexture(GL_TEXTURE_2D, m_grassTexture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    m_skybox.Initialize(5.0);
    m_skybox.LoadTextures("skybox/up.tga", "skybox/dn.tga", "skybox/ft.tga", "skybox/bk.tga", "skybox/lt.tga", "skybox/rt.tga");
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    delete [] m_terrain;
    m_terrain = NULL;
    delete [] m_terrainUVs;
    m_terrainUVs = NULL;
    delete [] m_terrainColors;
    m_terrainColors = NULL;
    delete [] m_terrainIndices;
    m_terrainIndices = NULL;
    return true;
    m_skybox.Release();
    glDeleteTextures(1, &m_grassTexture);
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
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 2.0f, 3000.0f);
    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

void CGfxOpenGL::Prepare(float dt)
{
    if (m_rotateRight)
        m_angle += dt * ROTATION_SPEED;
    else if (m_rotateLeft)
        m_angle -= dt * ROTATION_SPEED;

    if (m_angle > 360.0)
        m_angle = 0.0;

    if (m_angle < 0.0)
        m_angle = 360.0;

    // animate the models
    m_rhinoModel.Animate(dt);
    m_centaurModel.Animate(dt);
    m_demonModel.Animate(dt);
}

void CGfxOpenGL::Render()
{
    glClearColor(0.7f, 0.7f, 0.9f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // set up the view
    glLoadIdentity();
    xyz_t eye = { 0, 0, 0 };
    eye.y = GetHeightAt(eye.x, eye.z) + EYE_HEIGHT;
    gluLookAt(eye.x, eye.y, eye.z, 0.0, eye.y, -1.0, 0.0, 1.0, 0.0);
    glRotatef(m_angle, 0, 1, 0);
    // determine the frustum for culling
    CalculateFrustum();
    // draw the world
    m_skybox.Render(eye.x, eye.y, eye.z);
    DrawTerrain();
    DrawCreatures();
}

bool CGfxOpenGL::InitializeTerrain()
{
    GLubyte *heightmap = new GLubyte[TERRAIN_SIZE * TERRAIN_SIZE];
    FILE *pFile = fopen(heightmapFilename, "rb");

    if (!pFile)
        return false;

    fread(heightmap, TERRAIN_SIZE * TERRAIN_SIZE, 1, pFile);
    fclose(pFile);
    m_terrain = new GLfloat[TERRAIN_SIZE * TERRAIN_SIZE * 3];
    m_terrainUVs = new GLfloat[TERRAIN_SIZE * TERRAIN_SIZE * 2];
    m_terrainColors = new GLfloat[TERRAIN_SIZE * TERRAIN_SIZE * 3];
    m_terrainIndices = new GLuint[TERRAIN_SIZE * 2 * (TERRAIN_SIZE - 1)];
    int xyz, uv, rgb;
    int index = 0;

    for (int z = 0; z < TERRAIN_SIZE; ++z) {
        for (int x = 0; x < TERRAIN_SIZE; ++x) {
            xyz = rgb = 3 * (z * TERRAIN_SIZE + x);
            uv = 2 * (z * TERRAIN_SIZE + x);
            GLfloat height = (GLfloat)heightmap[z * TERRAIN_SIZE + x] * TERRAIN_HEIGHT_SCALE;
            m_terrain[xyz] = (GLfloat)(x - (GLfloat)TERRAIN_SIZE / 2) * TERRAIN_SCALE;
            m_terrain[xyz + 1] = height;
            m_terrain[xyz + 2] = (GLfloat)(z - (GLfloat)TERRAIN_SIZE / 2) * TERRAIN_SCALE;
            float color = 0.5f + 0.5f * height / TERRAIN_MAX_HEIGHT;
            m_terrainColors[rgb] = m_terrainColors[rgb + 1] = m_terrainColors[rgb + 2] = color;
            m_terrainUVs[uv] = (GLfloat)x / TERRAIN_SIZE * 32;
            m_terrainUVs[uv + 1] = (GLfloat)z / TERRAIN_SIZE * 32;

            if (z < TERRAIN_SIZE - 1) {
                m_terrainIndices[index++] = TERRAIN_SIZE * (z + 1) + x;
                m_terrainIndices[index++] = TERRAIN_SIZE * z + x;
            }
        }
    }

    return true;
}

void CGfxOpenGL::DrawTerrain()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_terrain);
    glTexCoordPointer(2, GL_FLOAT, 0, m_terrainUVs);
    glColorPointer(3, GL_FLOAT, 0, m_terrainColors);
    glBindTexture(GL_TEXTURE_2D, m_grassTexture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    for (int i = 0; i < TERRAIN_SIZE - 1; ++i) {
        glDrawElements(GL_TRIANGLE_STRIP, TERRAIN_SIZE * 2, GL_UNSIGNED_INT, &m_terrainIndices[i * TERRAIN_SIZE * 2]);
    }

    /*
    // to render the entire terrain with a single OpenGL call, you could use the following
    // code during initialization to set up arrays for use with glMultiDrawElements()
    GLsizei count[TERRAIN_SIZE-1];
    GLuint *indices[TERRAIN_SIZE-1];

    for (int i = 0; i < TERRAIN_SIZE - 1; ++i)
    {
      count[i] = TERRAIN_SIZE * 2;
      indices[i] = &m_terrainIndices[i * TERRAIN_SIZE * 2];
    }

    // this would be the only line you would need during rendering
    glMultiDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, (const void**)indices, TERRAIN_SIZE-1);
    */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}


bool CGfxOpenGL::InitializeCreatures()
{
    srand(27);
    m_rhinoModel.Load("rhino\\tris.md2", "rhino\\rhino.tga");
    m_centaurModel.Load("centaur\\tris.md2", "centaur\\centaur.tga");
    m_demonModel.Load("necromicus\\tris.md2", "necromicus\\necromicus.tga");

    for (int i = 0; i < NUM_CREATURES; ++i) {
        GLfloat x, z;

        do {
            x = FRAND() * (float)TERRAIN_SIZE / 2 * CREATURE_SCALE;
            z = FRAND() * (float)TERRAIN_SIZE / 2 * CREATURE_SCALE;
        } while (x * x + z * z < CREATURE_MIN_DISTANCE * CREATURE_MIN_DISTANCE);

        m_rhinos[i][0] = x;
        m_rhinos[i][1] = GetHeightAt(x, z);
        m_rhinos[i][2] = z;
        m_rhinos[i][3] = FRAND() * 360.0f;

        do {
            x = FRAND() * (float)TERRAIN_SIZE / 2 * CREATURE_SCALE;
            z = FRAND() * (float)TERRAIN_SIZE / 2 * CREATURE_SCALE;
        } while (x * x + z * z < CREATURE_MIN_DISTANCE * CREATURE_MIN_DISTANCE);

        m_centaurs[i][0] = x;
        m_centaurs[i][1] = GetHeightAt(x, z);
        m_centaurs[i][2] = z;
        m_centaurs[i][3] = FRAND() * 360.0f;

        do {
            x = FRAND() * (float)TERRAIN_SIZE / 2 * CREATURE_SCALE;
            z = FRAND() * (float)TERRAIN_SIZE / 2 * CREATURE_SCALE;
        } while (x * x + z * z < CREATURE_MIN_DISTANCE * CREATURE_MIN_DISTANCE);

        m_demons[i][0] = x;
        m_demons[i][1] = GetHeightAt(x, z);
        m_demons[i][2] = z;
        m_demons[i][3] = FRAND() * 360.0f;
    }

    m_centaurModel.SetAnimation(CMD2Model::IDLE);
    m_demonModel.SetAnimation(CMD2Model::IDLE);
    m_rhinoModel.SetAnimation(CMD2Model::IDLE);
    return true;
}

void CGfxOpenGL::DrawCreatures()
{
    // iterate through all the creatures, checking to see if the creature's bounding sphere
    // intersects the view volume when frustum culling is enabled
    for (int i = 0; i < NUM_CREATURES; ++i) {
        m_rhinoModel.Rotate(m_rhinos[i][3]);
        m_rhinoModel.Move(m_rhinos[i][0], m_rhinos[i][1], m_rhinos[i][2]);

        if (!m_useCulling || SphereInFrustum(m_rhinoModel.GetBoundingSphere(), m_frustum)) {
            m_rhinoModel.Render();
        }

        m_centaurModel.Rotate(m_centaurs[i][3]);
        m_centaurModel.Move(m_centaurs[i][0], m_centaurs[i][1], m_centaurs[i][2]);

        if (!m_useCulling || SphereInFrustum(m_centaurModel.GetBoundingSphere(), m_frustum)) {
            m_centaurModel.Render();
        }

        m_demonModel.Rotate(m_demons[i][3]);
        m_demonModel.Move(m_demons[i][0], m_demons[i][1], m_demons[i][2]);

        if (!m_useCulling || SphereInFrustum(m_demonModel.GetBoundingSphere(), m_frustum)) {
            m_demonModel.Render();
        }
    }
}

// utility function to return the height at a given terrain location
GLfloat CGfxOpenGL::GetHeightAt(GLfloat x, GLfloat z)
{
    float scaledX = x / TERRAIN_SCALE + (float)TERRAIN_SIZE / 2;
    float scaledZ = z / TERRAIN_SCALE + (float)TERRAIN_SIZE / 2;
    int x0 = (int)scaledX;

    if (scaledX < 0)
        --x0;

    int z0 = (int)scaledZ;

    if (scaledZ < 0)
        --z0;

    int p0, p1, p2, p3;
    p0 = (z0 * TERRAIN_SIZE + x0) * 3 + 1;
    p1 = (z0 * TERRAIN_SIZE + x0 + 1) * 3 + 1;
    p2 = ((z0 + 1) * TERRAIN_SIZE + x0) * 3 + 1;
    p3 = ((z0 + 1) * TERRAIN_SIZE + x0 + 1) * 3 + 1;
    float fracX = scaledX - x0;
    float fracZ = scaledZ - z0;
    float xInterp0 = m_terrain[p0] + fracX * (m_terrain[p1] - m_terrain[p0]);
    float xInterp1 = m_terrain[p2] + fracX * (m_terrain[p3] - m_terrain[p2]);
    return xInterp0 + fracZ * (xInterp1 - xInterp0);
}


// extract a plane from a given matrix and row id
void ExtractPlane(plane_t &plane, GLfloat *mat, int row)
{
    int scale = (row < 0) ? -1 : 1;
    row = abs(row) - 1;
    // calculate plane coefficients from the matrix
    plane.A = mat[3] + scale * mat[row];
    plane.B = mat[7] + scale * mat[row + 4];
    plane.C = mat[11] + scale * mat[row + 8];
    plane.D = mat[15] + scale * mat[row + 12];
    // normalize the plane
    float length = sqrtf(plane.A * plane.A + plane.B * plane.B + plane.C * plane.C);
    plane.A /= length;
    plane.B /= length;
    plane.C /= length;
    plane.D /= length;
}


// determines the current view frustum
void CGfxOpenGL::CalculateFrustum()
{
    // get the projection and modelview matrices
    GLfloat projection[16];
    GLfloat modelview[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    // use OpenGL to multiply them
    glPushMatrix();
    glLoadMatrixf(projection);
    glMultMatrixf(modelview);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    glPopMatrix();
    // extract each plane
    ExtractPlane(m_frustum.l, modelview, 1);
    ExtractPlane(m_frustum.r, modelview, -1);
    ExtractPlane(m_frustum.b, modelview, 2);
    ExtractPlane(m_frustum.t, modelview, -2);
    ExtractPlane(m_frustum.n, modelview, 3);
    ExtractPlane(m_frustum.f, modelview, -3);
}
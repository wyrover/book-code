#include "CGfxOpenGL.h"
#include "CTargaImage.h"
#include <cstdlib>
#include <gl/glu.h>

const float BOX_SIZE = 5.0f;
const float SPHERE_SIZE = 0.3f;
const float PI = 3.14159f;
const int SPHERE_RESOLUTION = 6;

const float LIGHT_POSITION[] = { -12, 10, 10, 1.0};
const float NEG_LIGHT_POSITION[] = { -12, -20, 10, 1.0};

// returns a number ranging from -1.0 to 1.0
#define FRAND()   (((float)rand()-(float)rand())/RAND_MAX)

#define RAND_COLOR() ((float)rand()/RAND_MAX)

CGfxOpenGL::CGfxOpenGL()
{
    m_angle = 0;
    m_useVertexArrays = false;
    m_useList = false;
    m_useCVAs = false;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    GLeeInit();
    InitializeMarbles();
    glVertexPointer(3, GL_FLOAT, 0, m_positions);
    glNormalPointer(GL_FLOAT, 0, m_texCoords);
    glTexCoordPointer(3, GL_FLOAT, 0, m_texCoords);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    CTargaImage image;
    image.Load("marble.tga");
    glGenTextures(1, &m_marbleTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_marbleTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    for (int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image.GetWidth(), image.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    }

    image.Release();
    image.Load("brick.tga");
    glGenTextures(1, &m_brickTexture);
    glBindTexture(GL_TEXTURE_2D, m_brickTexture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
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
    gluPerspective(75.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);             // set modelview matrix
    glLoadIdentity();                       // reset modelview matrix
    m_windowWidth = width;
    m_windowHeight = height;
}

bool CGfxOpenGL::Shutdown()
{
    glDeleteTextures(1, &m_marbleTexture);
    glDeleteTextures(1, &m_brickTexture);
    return true;
}


void CGfxOpenGL::InitializeMarbles()
{
    for (int marble = 0; marble < NUM_MARBLES; ++marble) {
        m_marbles[marble].color.x = RAND_COLOR() / 2.0f + 0.5f;
        m_marbles[marble].color.y = RAND_COLOR() / 2.0f + 0.5f;
        m_marbles[marble].color.z = RAND_COLOR() / 2.0f + 0.5f;
        m_marbles[marble].position.x = FRAND() * (BOX_SIZE - SPHERE_SIZE);
        m_marbles[marble].position.y = FRAND() * (BOX_SIZE - SPHERE_SIZE);
        m_marbles[marble].position.z = FRAND() * (BOX_SIZE - SPHERE_SIZE);
        m_marbles[marble].direction.x = FRAND();
        m_marbles[marble].direction.y = FRAND();
        m_marbles[marble].direction.z = FRAND();
        m_marbles[marble].direction.Normalize();
        m_marbles[marble].direction = m_marbles[marble].direction * 4.0f;
    }

    GenerateSphere(SPHERE_SIZE, SPHERE_RESOLUTION);
}

void CGfxOpenGL::MoveMarbles(float dt)
{
    for (int n = 0; n < NUM_MARBLES; ++n) {
        Marble* pMarble = &m_marbles[n];
        pMarble->position.x += pMarble->direction.x * dt;
        pMarble->position.y += pMarble->direction.y * dt;
        pMarble->position.z += pMarble->direction.z * dt;

        if (fabs(pMarble->position.x) + SPHERE_SIZE > BOX_SIZE) {
            pMarble->direction.x *= -1.0;
            pMarble->position.x += pMarble->direction.x * dt;
        }

        if (fabs(pMarble->position.y) + SPHERE_SIZE > BOX_SIZE) {
            pMarble->direction.y *= -1.0;
            pMarble->position.y += pMarble->direction.y * dt;
        }

        if (fabs(pMarble->position.z) + SPHERE_SIZE > BOX_SIZE) {
            pMarble->direction.z *= -1.0;
            pMarble->position.z += pMarble->direction.z * dt;
        }
    }
}

void CGfxOpenGL::Prepare(float dt)
{
    if (!m_useList)
        MoveMarbles(dt);
    else
        m_angle += 30 * dt;
}

void CGfxOpenGL::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, BOX_SIZE + 1, BOX_SIZE * 3, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(m_angle, 0.0, 1.0, 0.0);

    if (m_useList) {
        // use the existing list if there is one
        if (m_list) {
            glCallList(m_list);
            return;
        } else { // otherwise, create a new one
            m_list = glGenLists(1);
            glNewList(m_list, GL_COMPILE_AND_EXECUTE);
        }
    }

    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION);
    DrawFloor();
    DrawReflection();
    DrawMarbles(GL_FALSE);
    DrawBox();

    if (m_useList)
        glEndList();
}


// sides should be depth sorted for correct display
void CGfxOpenGL::DrawBox()
{
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
    glDisable(GL_DEPTH_TEST);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    // back
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(BOX_SIZE, -BOX_SIZE, -BOX_SIZE);
    glVertex3f(-BOX_SIZE, -BOX_SIZE, -BOX_SIZE);
    glVertex3f(-BOX_SIZE, BOX_SIZE, -BOX_SIZE);
    glVertex3f(BOX_SIZE, BOX_SIZE, -BOX_SIZE);
    // right
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(BOX_SIZE, -BOX_SIZE, BOX_SIZE);
    glVertex3f(BOX_SIZE, -BOX_SIZE, -BOX_SIZE);
    glVertex3f(BOX_SIZE, BOX_SIZE, -BOX_SIZE);
    glVertex3f(BOX_SIZE, BOX_SIZE, BOX_SIZE);
    // left
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(-BOX_SIZE, -BOX_SIZE, -BOX_SIZE);
    glVertex3f(-BOX_SIZE, -BOX_SIZE, BOX_SIZE);
    glVertex3f(-BOX_SIZE, BOX_SIZE, BOX_SIZE);
    glVertex3f(-BOX_SIZE, BOX_SIZE, -BOX_SIZE);
    // front
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-BOX_SIZE, -BOX_SIZE, BOX_SIZE);
    glVertex3f(BOX_SIZE, -BOX_SIZE, BOX_SIZE);
    glVertex3f(BOX_SIZE, BOX_SIZE, BOX_SIZE);
    glVertex3f(-BOX_SIZE, BOX_SIZE, BOX_SIZE);
    // top
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(-BOX_SIZE, BOX_SIZE, BOX_SIZE);
    glVertex3f(BOX_SIZE, BOX_SIZE, BOX_SIZE);
    glVertex3f(BOX_SIZE, BOX_SIZE, -BOX_SIZE);
    glVertex3f(-BOX_SIZE, BOX_SIZE, -BOX_SIZE);
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}


void CGfxOpenGL::DrawMarbles(bool invert)
{
    GLfloat white[] = { 0.5f, 0.5f, 0.5f, 1.0 };
    glEnable(GL_CULL_FACE);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (m_useVertexArrays) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    for (int n = 0; n < NUM_MARBLES; ++n) {
        glColor3fv(m_marbles[n].color.v);
        glPushMatrix();
        glTranslatef(m_marbles[n].position.x, invert ? -BOX_SIZE * 2.0f - m_marbles[n].position.y : m_marbles[n].position.y, m_marbles[n].position.z);
        DrawSphere();
        glPopMatrix();
    }

    if (m_useVertexArrays) {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDisable(GL_TEXTURE_CUBE_MAP);
    glDisable(GL_CULL_FACE);
}


// procedurally generate a sphere
bool CGfxOpenGL::GenerateSphere(float radius, int resolution)
{
    if (resolution < 2)
        resolution = 2;

    m_numStrips = resolution;
    m_vertsPerStrip = resolution * 4 + 2;
    m_numVertices = resolution * resolution * 2 - 2 * (resolution - 1);
    // allocate memory for the vertex arrays
    m_positions = new vector_t[m_numVertices];
    m_texCoords = new vector_t[m_numVertices];
    m_indexArray = new GLuint[m_vertsPerStrip * m_numStrips];
    // Generate the dome
    int index = 0;
    // set up the top vertex
    m_positions[0].x = 0.0;
    m_positions[0].y = radius;
    m_positions[0].z = 0.0;
    m_texCoords[0].s = 0.0;
    m_texCoords[0].t = 1.0;
    m_texCoords[0].r = 0.0;

    for (int n = 0; n < resolution * 2; ++n) {
        m_indexArray[index++] = n + 1;
        m_indexArray[index++] = 0;
    }

    m_indexArray[index++] = 1;
    m_indexArray[index++] = 0;
    // set up increments
    float sweepStepSize = PI / float(resolution);
    float longitude = 0.0;
    float latitude = PI / 2.0f - sweepStepSize;
    int vertex = 1;

    for (int y = 0; y < resolution - 1; ++y) {
        longitude = 0.0;

        for (int xz = 0; xz < resolution * 2; ++xz) {
            m_positions[vertex].x = radius * cosf(latitude) * cosf(longitude);
            m_positions[vertex].y = radius * sinf(latitude);
            m_positions[vertex].z = radius * cosf(latitude) * sinf(longitude);
            vector_t vec(m_positions[vertex]);
            vec.Normalize();
            // Calculate the spherical texture coordinates
            m_texCoords[vertex].s = vec.x;
            m_texCoords[vertex].t = vec.y;
            m_texCoords[vertex].r = vec.z;

            if (y == resolution - 2)
                m_indexArray[index++] = m_numVertices - 1;
            else
                m_indexArray[index++] = vertex + resolution * 2;

            m_indexArray[index++] = vertex;
            vertex++;
            longitude += PI / (float)resolution;
        }

        // create duplicate vertices at the end of the strip to close it
        m_indexArray[index++] = vertex;
        m_indexArray[index++] = vertex - resolution * 2;
        latitude -= sweepStepSize;
    }

    // set up the bottom vertex
    m_positions[vertex].x = 0.0;
    m_positions[vertex].y = -radius;
    m_positions[vertex].z = 0.0;
    m_texCoords[vertex].s = 0.0;
    m_texCoords[vertex].t = -1.0;
    m_texCoords[vertex].r = 0.0;
    return true;
}


void CGfxOpenGL::DrawSphere()
{
    if (m_useVertexArrays) {
        for (int i = 0; i < m_numStrips; ++i) {
            glDrawElements(GL_TRIANGLE_STRIP, m_vertsPerStrip, GL_UNSIGNED_INT, &m_indexArray[i * m_vertsPerStrip]);
        }
    } else { // draw using immediate mode instead
        for (int i = 0; i < m_numStrips; ++i) {
            glBegin(GL_TRIANGLE_STRIP);

            for (int j = 0; j < m_vertsPerStrip; ++j) {
                int index = m_indexArray[i * m_vertsPerStrip + j];
                glNormal3fv(m_texCoords[index].v);
                glTexCoord3fv(m_texCoords[index].v);
                glVertex3fv(m_positions[index].v);
            }

            glEnd();
        }
    }
}


void CGfxOpenGL::DrawFloor()
{
    glNormal3f(0.0, 1.0, 0.0);
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex3f(-BOX_SIZE * 10, -BOX_SIZE, BOX_SIZE * 10);
    glTexCoord2i(5, 0);
    glVertex3f(BOX_SIZE * 10, -BOX_SIZE, BOX_SIZE * 10);
    glTexCoord2i(5, 5);
    glVertex3f(BOX_SIZE * 10, -BOX_SIZE, -BOX_SIZE * 10);
    glTexCoord2i(0, 5);
    glVertex3f(-BOX_SIZE * 10, -BOX_SIZE, -BOX_SIZE * 10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);
}


void CGfxOpenGL::DrawReflection()
{
    glLightfv(GL_LIGHT0, GL_POSITION, NEG_LIGHT_POSITION);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    DrawBoxBottom();
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    DrawMarbles(GL_TRUE);
    glDisable(GL_STENCIL_TEST);
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION);
}


void CGfxOpenGL::DrawBoxBottom()
{
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-BOX_SIZE, -BOX_SIZE, BOX_SIZE);
    glVertex3f(-BOX_SIZE, -BOX_SIZE, -BOX_SIZE);
    glVertex3f(BOX_SIZE, -BOX_SIZE, -BOX_SIZE);
    glVertex3f(BOX_SIZE, -BOX_SIZE, BOX_SIZE);
    glEnd();
}
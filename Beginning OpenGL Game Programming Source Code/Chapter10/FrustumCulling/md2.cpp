#include "md2.h"
#include <cstdio>
#include <cmath>
#include <cfloat>


const float SPHERE_PADDING = 10.0;

// this is a bit of a hack, since the sphere isn't guaranteed to completely
// surround the model. However, the results for this demo are satisfactory
void CalculateBoundingSphere(sphere_t &sphere, GLfloat miny, GLfloat maxy)
{
    sphere.center.x = 0;
    sphere.center.y = (maxy + miny) / 2.0f;
    sphere.center.z = 0;
    sphere.radius = maxy - sphere.center.y + SPHERE_PADDING;
}

// CMD2Model constructor
CMD2Model::CMD2Model()
{
    m_currentFrame = 0;   // current keyframe
    m_nextFrame = 1;      // next keyframe
    m_interpol = 0.0;     // interpolation percent
    m_tris = NULL;    // triangle indices
    m_texCoords = NULL;          // texture coordinate indices
    m_verts = NULL;  // vertices
    m_currentVerts = NULL;
    m_texID = 0;           // skin/texture
    m_state = IDLE;
    m_rotate = 0.0;
}

// CMD2Model destructor
CMD2Model::~CMD2Model()
{
    Unload();
}

// CMD2Model::Load()
// access: public
// desc: loads model and skin
bool CMD2Model::Load(char *modelFile, char *skinFile, GLfloat scale)
{
    // open the model file
    FILE *pFile = fopen(modelFile, "rb");

    if (pFile == NULL)
        return false;

    fread(&m_info, sizeof(modelHeader_t), 1, pFile);
    m_verts = new xyz_t[m_info.numXYZ * m_info.numFrames];
    char *buffer = new char[m_info.numFrames * m_info.framesize];
    fseek(pFile, m_info.offsetFrames, SEEK_SET);
    fread(buffer, m_info.numFrames, m_info.framesize, pFile);
    int i, j;
    frame_t *frame;
    xyz_t *pVerts;
    m_miny = FLT_MAX;
    GLfloat maxy = FLT_MIN;

    for (j = 0; j < m_info.numFrames; ++j) {
        frame = (frame_t*)&buffer[m_info.framesize * j];
        pVerts = (xyz_t*)&m_verts[m_info.numXYZ * j];

        for (i = 0; i < m_info.numXYZ; i++) {
            pVerts[i].x = scale * (frame->scale[0] * frame->fp[i].v[0] + frame->translate[0]);
            pVerts[i].z = scale * (frame->scale[1] * frame->fp[i].v[1] + frame->translate[1]);
            pVerts[i].y = scale * (frame->scale[2] * frame->fp[i].v[2] + frame->translate[2]);

            if (j == 0) {
                if (pVerts[i].y < m_miny)
                    m_miny = pVerts[i].y;

                if (pVerts[i].y > maxy)
                    maxy = pVerts[i].y;
            }
        }
    }

    CalculateBoundingSphere(m_boundingSphere, m_miny, maxy);
    Move(0, 0, 0);
    m_tris = new mesh_t[m_info.numTris];
    fseek(pFile, m_info.offsetTris, SEEK_SET);
    fread(m_tris, m_info.numTris, sizeof(mesh_t), pFile);
    stIndex_t *stTemp = new stIndex_t[m_info.numST];
    m_texCoords = new texCoord_t[m_info.numTris * 3];
    fseek(pFile, m_info.offsetST, SEEK_SET);
    fread(stTemp, m_info.numST, sizeof(stIndex_t), pFile);
    int index = 0;

    for (i = 0; i < m_info.numTris; i++) {
        m_texCoords[index].s = (float)stTemp[m_tris[i].stIndex[0]].s / m_info.skinwidth;
        m_texCoords[index++].t = 1.0f - (float)stTemp[m_tris[i].stIndex[0]].t / m_info.skinheight;
        m_texCoords[index].s = (float)stTemp[m_tris[i].stIndex[1]].s / m_info.skinwidth;
        m_texCoords[index++].t = 1.0f - (float)stTemp[m_tris[i].stIndex[1]].t / m_info.skinheight;
        m_texCoords[index].s = (float)stTemp[m_tris[i].stIndex[2]].s / m_info.skinwidth;
        m_texCoords[index++].t = 1.0f - (float)stTemp[m_tris[i].stIndex[2]].t / m_info.skinheight;
    }

    m_currentVerts = new xyz_t[m_info.numTris * 3];
    // close file and free memory
    fclose(pFile);
    delete[] buffer;
    CTargaImage image;
    image.Load(skinFile);
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();
    m_currentFrame = 0;
    m_nextFrame = 1;
    m_interpol = 0.0;
    // animate at least one frame to make sure that m_currentVerts gets initialized
    SetAnimation(IDLE);
    Animate(0);
    return TRUE;
}

void CMD2Model::SetAnimation(int state, int nextState)
{
    switch (state) {
    case RUN:
        m_startFrame = 40;
        m_endFrame = 45;
        break;

    case ATTACK:
        m_startFrame = 46;
        m_endFrame = 53;
        break;

    case PAIN1:
        m_startFrame = 54;
        m_endFrame = 57;
        break;

    case PAIN2:
        m_startFrame = 58;
        m_endFrame = 61;
        break;

    case PAIN3:
        m_startFrame = 62;
        m_endFrame = 65;
        break;

    case JUMP:
        m_startFrame = 66;
        m_endFrame = 71;
        break;

    case FLIPOFF:
        m_startFrame = 72;
        m_endFrame = 83;
        break;

    case SAULTE:
        m_startFrame = 84;
        m_endFrame = 94;
        break;

    case TAUNT:
        m_startFrame = 95;
        m_endFrame = 111;
        break;

    case WAVE:
        m_startFrame = 112;
        m_endFrame = 122;
        break;

    case POINT:
        m_startFrame = 123;
        m_endFrame = 134;
        break;

    case CROUCH_IDLE:
        m_startFrame = 135;
        m_endFrame = 153;
        break;

    case CROUCH_WALK:
        m_startFrame = 154;
        m_endFrame = 159;
        break;

    case CROUCH_ATTACK:
        m_startFrame = 160;
        m_endFrame = 168;
        break;

    case CROUCH_PAIN:
        m_startFrame = 169;
        m_endFrame = 172;
        break;

    case CROUCH_DEATH:
        m_startFrame = 173;
        m_endFrame = 177;
        break;

    case DEATH1:
        m_startFrame = 178;
        m_endFrame = 183;
        break;

    case DEATH2:
        m_startFrame = 184;
        m_endFrame = 189;
        break;

    case DEATH3:
        m_startFrame = 190;
        m_endFrame = 197;
        break;

    case IDLE:
        m_startFrame = 0;
        m_endFrame = 39;
        break;

    default:
        return;
    }

    if (m_state != state) {
        m_currentFrame = m_startFrame;
        m_nextFrame = m_startFrame + 1;
    }

    m_state = state;
    m_nextState = nextState;
}

void CMD2Model::SetAnimationCustom(int start, int end)
{
    if (start < 0)
        start = 0;

    if (start >= m_info.numFrames)
        start = m_info.numFrames - 1;

    if (end < 0)
        end = 0;

    if (end >= m_info.numFrames)
        end = m_info.numFrames - 1;

    m_startFrame = start;
    m_endFrame = end;
    m_state = _CUSTOM;
    m_nextState = _CUSTOM;
}

void CMD2Model::Animate(float seconds)
{
    xyz_t *vList;              // current frame vertices
    xyz_t *nextVList;          // next frame vertices
    int i;                                  // index counter
    float x1, y1, z1;                  // current frame point values
    float x2, y2, z2;                  // next frame point values

    if (m_startFrame > m_currentFrame) {
        m_currentFrame = m_startFrame;
    }

    if (m_startFrame < 0 || m_endFrame < 0)
        return;

    if (m_startFrame >= m_info.numFrames || m_endFrame >= m_info.numFrames)
        return;

    m_interpol += KEYFRAMES_PER_S * seconds;  // increase percentage of interpolation between frames

    if (m_interpol >= 1.0) {
        m_interpol = 0.0f;
        m_currentFrame++;
        m_nextFrame = m_currentFrame + 1;

        if (m_currentFrame > m_endFrame) {
            if (m_nextState == _REPEAT || m_state == _CUSTOM) {
                m_currentFrame = m_startFrame;
                m_nextFrame = m_currentFrame + 1;
            } else {
                SetAnimation(m_nextState, _REPEAT);
            }
        }

        if (m_nextFrame > m_endFrame)
            m_nextFrame = m_startFrame;
    }

    vList = &m_verts[m_info.numXYZ * m_currentFrame];
    nextVList = &m_verts[m_info.numXYZ * m_nextFrame];

    for (i = 0; i < m_info.numTris * 3; i++) {
        xyz_t &vertex = m_currentVerts[i];
        // get first points of each frame
        x1 = vList[m_tris[i / 3].meshIndex[i % 3]].x;
        y1 = vList[m_tris[i / 3].meshIndex[i % 3]].y;
        z1 = vList[m_tris[i / 3].meshIndex[i % 3]].z;
        x2 = nextVList[m_tris[i / 3].meshIndex[i % 3]].x;
        y2 = nextVList[m_tris[i / 3].meshIndex[i % 3]].y;
        z2 = nextVList[m_tris[i / 3].meshIndex[i % 3]].z;
        // store first interpolated vertex of triangle
        vertex.x = x1 + m_interpol * (x2 - x1);
        vertex.y = y1 + m_interpol * (y2 - y1);
        vertex.z = z1 + m_interpol * (z2 - z1);
    }
}

void CMD2Model::Render()
{
    glPushMatrix();
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glRotatef(m_rotate, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_currentVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords);
    glDrawArrays(GL_TRIANGLES, 0, m_info.numTris * 3);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

// Unload()
// desc: unloads model data from memory
void CMD2Model::Unload()
{
    delete [] m_tris;
    m_tris = NULL;
    delete [] m_verts;
    m_verts = NULL;
    delete [] m_texCoords;
    m_texCoords = NULL;
    delete [] m_currentVerts;
    m_currentVerts = NULL;
    glDeleteTextures(1, &m_texID);
}


void CMD2Model::Move(GLfloat x, GLfloat y, GLfloat z)
{
    m_pos.x = x;
    m_pos.y = y - m_miny;
    m_pos.z = z;
    m_boundingSphere.center.x = x;
    m_boundingSphere.center.y = y + m_boundingSphere.radius - SPHERE_PADDING;
    m_boundingSphere.center.z = z;
}
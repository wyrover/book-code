#include "md2.h"
#include <cstdio>
#include <cmath>
#include <cfloat>


const float SPHERE_PADDING = 10.0;


struct modelInfo_t {
    int numVerts;
    int numTris;
    int numFrames;
    GLfloat minY;
    sphere_t boundingSphere;
};


bool LoadModel(char *filename, xyz_t* &verts, mesh_t* &tris, texCoord_t* &texCoords, modelInfo_t &info, GLfloat scale);
void CalculateBoundingSphere(sphere_t &sphere, GLfloat miny, GLfloat maxy);

// CMD2Instance constructor
CMD2Instance::CMD2Instance()
{
    m_currentFrame = 0;   // current keyframe
    m_nextFrame = 1;      // next keyframe
    m_interpol = 0.0;     // interpolation percent
    m_currentVerts = NULL;
    m_state = IDLE;
    m_rotate = 0.0;
    m_pData = NULL;
    m_numWeaponVertices = 0;
    m_numVertices = 0;
}

// CMD2Instance destructor
CMD2Instance::~CMD2Instance()
{
    Unload();
}

void CMD2Instance::SetAnimation(int state, int nextState)
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

void CMD2Instance::SetAnimationCustom(int start, int end)
{
    if (start < 0)
        start = 0;

    if (start >= m_numFrames)
        start = m_numFrames - 1;

    if (end < 0)
        end = 0;

    if (end >= m_numFrames)
        end = m_numFrames - 1;

    m_startFrame = start;
    m_endFrame = end;
    m_state = _CUSTOM;
    m_nextState = _CUSTOM;
}

void CMD2Instance::Animate(float seconds)
{
    if (m_startFrame > m_currentFrame) {
        m_currentFrame = m_startFrame;
    }

    if (m_startFrame < 0 || m_endFrame < 0)
        return;

    if (m_startFrame >= m_numFrames || m_endFrame >= m_numFrames)
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
            } else if (m_nextState == _STATIC) {
                m_currentFrame = m_nextFrame = m_startFrame = m_endFrame;
                m_state = _STATIC;
            } else {
                SetAnimation(m_nextState, _REPEAT);
            }
        }

        if (m_nextFrame > m_endFrame)
            m_nextFrame = m_startFrame;
    }

    if ((m_nextState == _STATIC) && (m_nextFrame == m_startFrame))
        m_nextFrame = m_endFrame;

    m_pData->Animate(m_currentVerts, m_weaponVerts, m_currentFrame, m_nextFrame, m_interpol);
}

void CMD2Instance::Render()
{
    glPushMatrix();
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glRotatef(m_rotate, 0, 1, 0);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_currentVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, m_pData->m_texCoords);
    glBindTexture(GL_TEXTURE_2D, m_pData->m_texID);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

    if (m_weaponVerts) {
        glVertexPointer(3, GL_FLOAT, 0, m_weaponVerts);
        glTexCoordPointer(2, GL_FLOAT, 0, m_pData->m_weaponTexCoords);
        glBindTexture(GL_TEXTURE_2D, m_pData->m_weaponTexID);
        glDrawArrays(GL_TRIANGLES, 0, m_numWeaponVertices);
    }

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

// Unload()
// desc: unloads model data from memory
void CMD2Instance::Unload()
{
    delete [] m_currentVerts;
    m_currentVerts = NULL;
    delete [] m_weaponVerts;
    m_weaponVerts = NULL;
}


void CMD2Instance::Move(GLfloat x, GLfloat y, GLfloat z)
{
    m_pos.x = x;
    m_pos.y = y - m_miny;
    m_pos.z = z;
    m_boundingSphere.center.x = x;
    m_boundingSphere.center.y = y + m_boundingSphere.radius - SPHERE_PADDING;
    m_boundingSphere.center.z = z;
}

// CMD2Data constructor
CMD2Data::CMD2Data()
{
    m_tris = NULL;    // triangle indices
    m_texCoords = NULL;          // texture coordinate indices
    m_verts = NULL;  // vertices
    m_texID = 0;           // skin/texture
    m_numVerts = 0;
    m_numTris = 0;
    m_numFrames = 0;
    m_weaponTris = NULL;
    m_weaponTexCoords = NULL;
    m_weaponVerts = NULL;
    m_numWeaponVerts = 0;
    m_numWeaponTris = 0;
    m_weaponTexID = 0;
}

// CMD2Data destructor
CMD2Data::~CMD2Data()
{
    Unload();
}

// CMD2Data::Load()
// access: public
// desc: loads model and skin
bool CMD2Data::Load(char *modelFile, char *skinFile, char *weaponFile, char *weaponSkin, float scale)
{
    modelInfo_t info;
    LoadModel(modelFile, m_verts, m_tris, m_texCoords, info, scale);
    m_numVerts = info.numVerts;
    m_numTris = info.numTris;
    m_numFrames = info.numFrames;
    m_miny = info.minY;
    m_boundingSphere = info.boundingSphere;

    if (weaponFile) {
        LoadModel(weaponFile, m_weaponVerts, m_weaponTris, m_weaponTexCoords, info, scale);
        m_numWeaponVerts = info.numVerts;
        m_numWeaponTris = info.numTris;
    }

    CTargaImage image;
    image.Load(skinFile);
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
    image.Release();

    if (weaponSkin) {
        image.Load(weaponSkin);
        glGenTextures(1, &m_weaponTexID);
        glBindTexture(GL_TEXTURE_2D, m_weaponTexID);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.GetWidth(), image.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.GetImage());
        image.Release();
    }

    return true;
}

CMD2Instance* CMD2Data::GetInstance()
{
    CMD2Instance *pInstance = new CMD2Instance();
    pInstance->m_currentVerts = new xyz_t[m_numTris * 3];
    pInstance->m_weaponVerts = new xyz_t[m_numWeaponTris * 3];
    pInstance->m_currentFrame = 0;
    pInstance->m_nextFrame = 1;
    pInstance->m_interpol = 0.0;
    pInstance->m_numFrames = m_numFrames;
    pInstance->m_numVertices = m_numTris * 3;
    pInstance->m_numWeaponVertices = m_numWeaponTris * 3;
    pInstance->m_pData = this;
    pInstance->m_boundingSphere = m_boundingSphere;
    pInstance->m_miny = m_miny;
    // animate at least one frame to make sure that m_currentVerts gets initialized
    pInstance->SetAnimation(CMD2Instance::IDLE);
    pInstance->Animate(0);
    return pInstance;
}


void CMD2Data::Animate(xyz_t *pVerts, xyz_t *pWeaponVerts, int curFrame, int nextFrame, float interpol)
{
    xyz_t *vList;              // current frame vertices
    xyz_t *nextVList;          // next frame vertices
    int i;                                  // index counter
    float x1, y1, z1;                  // current frame point values
    float x2, y2, z2;                  // next frame point values
    vList = &m_verts[m_numVerts * curFrame];
    nextVList = &m_verts[m_numVerts * nextFrame];

    for (i = 0; i < m_numTris * 3; i++) {
        xyz_t &vertex = pVerts[i];
        // get first points of each frame
        x1 = vList[m_tris[i / 3].meshIndex[i % 3]].x;
        y1 = vList[m_tris[i / 3].meshIndex[i % 3]].y;
        z1 = vList[m_tris[i / 3].meshIndex[i % 3]].z;
        x2 = nextVList[m_tris[i / 3].meshIndex[i % 3]].x;
        y2 = nextVList[m_tris[i / 3].meshIndex[i % 3]].y;
        z2 = nextVList[m_tris[i / 3].meshIndex[i % 3]].z;
        // store first interpolated vertex of triangle
        vertex.x = x1 + interpol * (x2 - x1);
        vertex.y = y1 + interpol * (y2 - y1);
        vertex.z = z1 + interpol * (z2 - z1);
    }

    if (m_weaponVerts) {
        vList = &m_weaponVerts[m_numWeaponVerts * curFrame];
        nextVList = &m_weaponVerts[m_numWeaponVerts * nextFrame];

        for (i = 0; i < m_numWeaponTris * 3; i++) {
            xyz_t &vertex = pWeaponVerts[i];
            // get first points of each frame
            x1 = vList[m_weaponTris[i / 3].meshIndex[i % 3]].x;
            y1 = vList[m_weaponTris[i / 3].meshIndex[i % 3]].y;
            z1 = vList[m_weaponTris[i / 3].meshIndex[i % 3]].z;
            x2 = nextVList[m_weaponTris[i / 3].meshIndex[i % 3]].x;
            y2 = nextVList[m_weaponTris[i / 3].meshIndex[i % 3]].y;
            z2 = nextVList[m_weaponTris[i / 3].meshIndex[i % 3]].z;
            // store first interpolated vertex of triangle
            vertex.x = x1 + interpol * (x2 - x1);
            vertex.y = y1 + interpol * (y2 - y1);
            vertex.z = z1 + interpol * (z2 - z1);
        }
    }
}

// Unload()
// desc: unloads model data from memory
void CMD2Data::Unload()
{
    delete [] m_tris;
    m_tris = NULL;
    delete [] m_verts;
    m_verts = NULL;
    delete [] m_texCoords;
    m_texCoords = NULL;
    delete [] m_weaponTris;
    m_weaponTris = NULL;
    delete [] m_weaponVerts;
    m_weaponVerts = NULL;
    delete [] m_weaponTexCoords;
    m_weaponTexCoords = NULL;
    glDeleteTextures(1, &m_texID);
}


bool LoadModel(char *filename, xyz_t* &verts, mesh_t* &tris, texCoord_t* &texCoords, modelInfo_t &info, GLfloat scale)
{
    // open the model file
    FILE *pFile = fopen(filename, "rb");

    if (pFile == NULL)
        return false;

    modelHeader_t header;
    fread(&header, sizeof(modelHeader_t), 1, pFile);
    verts = new xyz_t[header.numXYZ * header.numFrames];
    info.numVerts = header.numXYZ;
    info.numTris = header.numTris;
    info.numFrames = header.numFrames;
    char *buffer = new char[header.numFrames * header.framesize];
    fseek(pFile, header.offsetFrames, SEEK_SET);
    fread(buffer, header.numFrames, header.framesize, pFile);
    int i, j;
    frame_t *frame;
    xyz_t *pVerts;
    info.minY = FLT_MAX;
    GLfloat maxy = FLT_MIN;

    for (j = 0; j < header.numFrames; ++j) {
        frame = (frame_t*)&buffer[header.framesize * j];
        pVerts = (xyz_t*)&verts[header.numXYZ * j];

        for (i = 0; i < header.numXYZ; i++) {
            pVerts[i].x = scale * (frame->scale[0] * frame->fp[i].v[0] + frame->translate[0]);
            pVerts[i].z = scale * (frame->scale[1] * frame->fp[i].v[1] + frame->translate[1]);
            pVerts[i].y = scale * (frame->scale[2] * frame->fp[i].v[2] + frame->translate[2]);

            if (j == 0) {
                if (pVerts[i].y < info.minY)
                    info.minY = pVerts[i].y;

                if (pVerts[i].y > maxy)
                    maxy = pVerts[i].y;
            }
        }
    }

    CalculateBoundingSphere(info.boundingSphere, info.minY, maxy);
    tris = new mesh_t[header.numTris];
    fseek(pFile, header.offsetTris, SEEK_SET);
    fread(tris, header.numTris, sizeof(mesh_t), pFile);
    stIndex_t *stTemp = new stIndex_t[header.numST];
    texCoords = new texCoord_t[header.numTris * 3];
    fseek(pFile, header.offsetST, SEEK_SET);
    fread(stTemp, header.numST, sizeof(stIndex_t), pFile);
    int index = 0;

    for (i = 0; i < header.numTris; i++) {
        texCoords[index].s = (float)stTemp[tris[i].stIndex[0]].s / header.skinwidth;
        texCoords[index++].t = 1.0f - (float)stTemp[tris[i].stIndex[0]].t / header.skinheight;
        texCoords[index].s = (float)stTemp[tris[i].stIndex[1]].s / header.skinwidth;
        texCoords[index++].t = 1.0f - (float)stTemp[tris[i].stIndex[1]].t / header.skinheight;
        texCoords[index].s = (float)stTemp[tris[i].stIndex[2]].s / header.skinwidth;
        texCoords[index++].t = 1.0f - (float)stTemp[tris[i].stIndex[2]].t / header.skinheight;
    }

    // close file and free memory
    fclose(pFile);
    delete[] buffer;
    return true;
}

// this is a bit of a hack, since the sphere isn't guaranteed to completely
// surround the model. However, the results for this demo are satisfactory
void CalculateBoundingSphere(sphere_t &sphere, GLfloat miny, GLfloat maxy)
{
    sphere.center.x = 0;
    sphere.center.y = (maxy + miny) / 2.0f;
    sphere.center.z = 0;
    sphere.radius = maxy - sphere.center.y + SPHERE_PADDING;
}


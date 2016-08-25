#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#include "glee.h"
#include <cmath>

const int NUM_MARBLES = 75;

struct vector_t {
    union {
        struct {
            float x, y, z;
        };
        struct {
            float s, t, r;
        };
        float v[3];
    };

    vector_t() : x(0), y(0), z(0) {}
    vector_t(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

    vector_t operator*(const float s) const
    {
        return vector_t(this->x * s, this->y * s, this->z * s);
    }

    void Normalize()
    {
        GLfloat length = sqrtf(x * x + y * y + z * z);

        if (length > 0.0f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }
};

struct Marble {
    vector_t position;
    vector_t color;
    vector_t direction;
};



class CGfxOpenGL
{
public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    void SetupProjection(int width, int height);
    bool Shutdown();
    void Prepare(float dt);
    void Render();

    bool ToggleVertexArrays()
    {
        m_useVertexArrays = !m_useVertexArrays;

        if (m_useList) {
            glDeleteLists(m_list, 1);
            m_list = 0;
        }

        return m_useVertexArrays;
    }

    bool ToggleDisplayList()
    {
        m_useList = !m_useList;

        if (m_useList) {
            glDeleteLists(m_list, 1);
            m_list = 0;
        }

        return m_useList;
    }

    bool ToggleCVAs()
    {
        m_useCVAs = !m_useCVAs;

        if (GLEE_EXT_compiled_vertex_array) {
            if (m_useCVAs) {
                glLockArraysEXT(0, m_numVertices);
            } else {
                glUnlockArraysEXT();
            }
        }

        return m_useCVAs;
    }

    void RotateRight()
    {
        m_angle -= 1.0f;
    }
    void RotateLeft()
    {
        m_angle += 1.0f;
    }

protected:
    void InitializeMarbles();
    void MoveMarbles(float dt);
    bool GenerateSphere(float radius, int resolution);
    void DrawBox();
    void DrawMarbles(bool invert);
    void DrawSphere();
    void DrawFloor();
    void DrawBoxBottom();
    void DrawReflection();

private:
    int m_windowWidth;
    int m_windowHeight;

    int m_numStrips;
    int m_vertsPerStrip;

    bool m_useList;

    GLuint m_numVertices;
    GLuint m_list;

    bool m_useVertexArrays;
    bool m_useCVAs;

    GLuint m_marbleTexture;
    GLuint m_brickTexture;

    GLfloat m_angle;

    GLuint *m_indexArray;
    vector_t  *m_positions;
    vector_t  *m_texCoords;

    Marble m_marbles[NUM_MARBLES];
};

#endif
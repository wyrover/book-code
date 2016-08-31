#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include "CGfxOpenGL.h"
#include <cmath>

const float DISK_DEG_PER_S = 90.0f;
const float DISK_LIMIT = 3.0;


CGfxOpenGL::CGfxOpenGL()
{
    m_diskPos = 0.0;
    m_diskRot = 0.0;
    m_pDisk = NULL;
    m_srcFactor = GL_SRC_ALPHA;
    m_dstFactor = GL_ONE_MINUS_SRC_ALPHA;
    m_blendEquation = GL_FUNC_ADD;
    myglBlendEquation = NULL;
    myglBlendColor = NULL;
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glEnable(GL_DEPTH_TEST);
    m_pDisk = gluNewQuadric();
    // initialize function pointers
    myglBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress("glBlendEquationEXT");
    myglBlendColor = (PFNGLBLENDCOLORPROC) wglGetProcAddress("glBlendColorEXT");

    if (myglBlendColor) {
        myglBlendColor(0.5f, 0.5f, 0.5f, 0.5f);
    }

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
    if (m_pDisk)
        gluDeleteQuadric(m_pDisk);

    m_pDisk = NULL;
    return true;
}


void CGfxOpenGL::Prepare(float dt)
{
    // used to move and rotate the disk
    static float increment = 0.4f;

    if (dt > 1.0)
        dt = 1.0;

    if (m_diskPos > DISK_LIMIT || m_diskPos < -DISK_LIMIT)
        increment *= -1.0f;

    m_diskPos += increment * dt;
    m_diskRot += DISK_DEG_PER_S * dt;

    if (m_diskRot > 360.0f)
        m_diskRot = 0.0f;
}

void CGfxOpenGL::Render()
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // set up the camera
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // set up the blend function and equation
    glBlendFunc(m_srcFactor, m_dstFactor);

    if (myglBlendEquation)
        myglBlendEquation(m_blendEquation);

    // draw the background disk
    glColor4f(0.4f, 0.8f, 0.6f, 0.7f);
    gluDisk(m_pDisk, 0.0, 4.0, 64, 16);
    // draw the moving disk
    glEnable(GL_BLEND);
    glTranslatef(0.0, m_diskPos, 2.0);
    glRotatef(m_diskRot, 1.0, 0.0, 0.0);
    glColor4f(0.7f, 0.3f, 0.5f, 0.6f);
    gluDisk(m_pDisk, 0.5, 2.0, 32, 16);
    glDisable(GL_BLEND);
}


void CGfxOpenGL::NextBlendEquation()
{
    switch (m_blendEquation) {
    case GL_FUNC_ADD:
        m_blendEquation = GL_FUNC_SUBTRACT;
        break;

    case GL_FUNC_SUBTRACT:
        m_blendEquation = GL_FUNC_REVERSE_SUBTRACT;
        break;

    case GL_FUNC_REVERSE_SUBTRACT:
        m_blendEquation = GL_MIN;
        break;

    case GL_MIN:
        m_blendEquation = GL_MAX;
        break;

    case GL_MAX:
        m_blendEquation = GL_FUNC_ADD;
        break;
    }
}

void CGfxOpenGL::NextSourceFactor()
{
    switch (m_srcFactor) {
    case GL_ONE:
        m_srcFactor = GL_SRC_COLOR;
        break;

    case GL_SRC_ALPHA_SATURATE:
        m_srcFactor = GL_CONSTANT_COLOR;
        break;

    case GL_ONE_MINUS_CONSTANT_ALPHA:
        m_srcFactor = GL_ZERO;
        break;

    default:
        m_srcFactor++;
        break;
    }
}

void CGfxOpenGL::NextDestFactor()
{
    switch (m_dstFactor) {
    case GL_ONE:
        m_dstFactor = GL_SRC_COLOR;
        break;

    case GL_ONE_MINUS_DST_COLOR:
        m_dstFactor = GL_CONSTANT_COLOR;
        break;

    case GL_ONE_MINUS_CONSTANT_ALPHA:
        m_dstFactor = GL_ZERO;
        break;

    default:
        m_dstFactor++;
        break;
    }
}

const char* CGfxOpenGL::GetBlendEquation()
{
    switch (m_blendEquation) {
    case GL_FUNC_ADD:
        return "GL_FUNC_ADD";

    case GL_FUNC_SUBTRACT:
        return "GL_FUNC_SUBTRACT";

    case GL_FUNC_REVERSE_SUBTRACT:
        return "GL_FUNC_REVERSE_SUBTRACT";

    case GL_MIN:
        return "GL_MIN";

    case GL_MAX:
        return "GL_MAX";

    default:
        return "Something's wrong here...";
    }
}

const char* CGfxOpenGL::GetSourceFactor()
{
    switch (m_srcFactor) {
    case GL_ZERO:
        return "GL_ZERO";

    case GL_ONE:
        return "GL_ONE";

    case GL_SRC_COLOR:
        return "GL_SRC_COLOR";

    case GL_ONE_MINUS_SRC_COLOR:
        return "GL_ONE_MINUS_SRC_COLOR";

    case GL_SRC_ALPHA:
        return "GL_SRC_ALPHA";

    case GL_ONE_MINUS_SRC_ALPHA:
        return "GL_ONE_MINUS_SRC_ALPHA";

    case GL_DST_ALPHA:
        return "GL_DST_ALPHA";

    case GL_ONE_MINUS_DST_ALPHA:
        return "GL_ONE_MINUS_DST_ALPHA";

    case GL_DST_COLOR:
        return "GL_DST_COLOR";

    case GL_ONE_MINUS_DST_COLOR:
        return "GL_ONE_MINUS_DST_COLOR";

    case GL_SRC_ALPHA_SATURATE:
        return "GL_SRC_ALPHA_SATURATE";

    case GL_CONSTANT_COLOR:
        return "GL_CONSTANT_COLOR";

    case GL_ONE_MINUS_CONSTANT_COLOR:
        return "GL_ONE_MINUS_CONSTANT_COLOR";

    case GL_CONSTANT_ALPHA:
        return "GL_CONSTANT_ALPHA";

    case GL_ONE_MINUS_CONSTANT_ALPHA:
        return "GL_ONE_MINUS_CONSTANT_ALPHA";

    default:
        return "Something's wrong here...";
    }
}

const char* CGfxOpenGL::GetDestFactor()
{
    switch (m_dstFactor) {
    case GL_ZERO:
        return "GL_ZERO";

    case GL_ONE:
        return "GL_ONE";

    case GL_SRC_COLOR:
        return "GL_SRC_COLOR";

    case GL_ONE_MINUS_SRC_COLOR:
        return "GL_ONE_MINUS_SRC_COLOR";

    case GL_SRC_ALPHA:
        return "GL_SRC_ALPHA";

    case GL_ONE_MINUS_SRC_ALPHA:
        return "GL_ONE_MINUS_SRC_ALPHA";

    case GL_DST_ALPHA:
        return "GL_DST_ALPHA";

    case GL_ONE_MINUS_DST_ALPHA:
        return "GL_ONE_MINUS_DST_ALPHA";

    case GL_DST_COLOR:
        return "GL_DST_COLOR";

    case GL_ONE_MINUS_DST_COLOR:
        return "GL_ONE_MINUS_DST_COLOR";

    case GL_CONSTANT_COLOR:
        return "GL_CONSTANT_COLOR";

    case GL_ONE_MINUS_CONSTANT_COLOR:
        return "GL_ONE_MINUS_CONSTANT_COLOR";

    case GL_CONSTANT_ALPHA:
        return "GL_CONSTANT_ALPHA";

    case GL_ONE_MINUS_CONSTANT_ALPHA:
        return "GL_ONE_MINUS_CONSTANT_ALPHA";

    default:
        return "Something's wrong here...";
    }
}



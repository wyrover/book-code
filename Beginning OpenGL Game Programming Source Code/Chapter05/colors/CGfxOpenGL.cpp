#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "CGfxOpenGL.h"


CGfxOpenGL::CGfxOpenGL()
{
    myglSecondaryColor3f = NULL;
    memset(vertices, 0, sizeof(vertices));
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // initialize the extension function pointer
    myglSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC) wglGetProcAddress("glSecondaryColor3fEXT");
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    return true;
}


void CGfxOpenGL::Prepare(float dt)
{
}

void CGfxOpenGL::Render()
{
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // draw the four quads
    glShadeModel(GL_FLAT);
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(1.0, 0.0, 0.0);
    glVertex2i(1, 6);
    glColor3d(0.0, 1.0, 0.0);
    glVertex2i(4, 6);
    glColor3d(0.0, 0.0, 1.0);
    glVertex2i(4, 9);
    glColor3d(1.0, 1.0, 1.0);
    glVertex2i(1, 9);
    glEnd();
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(1.0, 0.0, 0.0);
    glVertex2i(1, 1);
    glColor3d(0.0, 1.0, 0.0);
    glVertex2i(4, 1);
    glColor3d(0.0, 0.0, 1.0);
    glVertex2i(4, 4);
    glColor3d(1.0, 1.0, 1.0);
    glVertex2i(1, 4);
    glEnd();
    glEnable(GL_COLOR_SUM);
    glShadeModel(GL_FLAT);
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(1.0, 0.0, 0.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(0.0, 1.0, 0.0);

    glVertex2i(6, 6);
    glColor3d(0.0, 1.0, 0.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(0.0, 0.0, 1.0);

    glVertex2i(9, 6);
    glColor3d(0.0, 0.0, 1.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(1.0, 0.0, 0.0);

    glVertex2i(9, 9);
    glColor3d(1.0, 1.0, 1.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(0.0, 0.0, 0.0);

    glVertex2i(6, 9);
    glEnd();
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLE_FAN);
    glColor3d(1.0, 0.0, 0.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(0.0, 1.0, 0.0);

    glVertex2i(6, 1);
    glColor3d(0.0, 1.0, 0.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(0.0, 0.0, 1.0);

    glVertex2i(9, 1);
    glColor3d(0.0, 0.0, 1.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(1.0, 0.0, 0.0);

    glVertex2i(9, 4);
    glColor3d(1.0, 1.0, 1.0);

    if (myglSecondaryColor3f)
        myglSecondaryColor3f(0.0, 0.0, 0.0);

    glVertex2i(6, 4);
    glEnd();
    glDisable(GL_COLOR_SUM);
}


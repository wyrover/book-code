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
#include "vector.h"
#include "Plane.h"

#include "CTargaImage.h"
#include "md2.h"
#include "TextureMgr.h"
#include "ChessGame.h"
#include "CGfxOpenGL.h"

#pragma warning(disable:4244)
#pragma warning(disable:4305)   // disable double/float truncation


CGfxOpenGL::CGfxOpenGL()
{
}

CGfxOpenGL::~CGfxOpenGL()
{
}

bool CGfxOpenGL::Init()
{
    if (!m_texMgr.LoadTextures("textures.dat"))
        return false;

    glEnable(GL_TEXTURE_2D);
    m_tableTexture = m_texMgr.GetTextureID("wood");
    m_boardTexture = m_texMgr.GetTextureID("chessboard");
    m_texMgr.Bind(m_boardTexture);
    GenerateChessBoardDL();
    m_whiteViewPos = CVector(-1.0, 8.0, 4.0);
    m_blackViewPos = CVector(9.0, 8.0, 4.0);
    m_currentView = WHITE;
    return true;
}

bool CGfxOpenGL::Shutdown()
{
    m_texMgr.ReleaseTextures();
    glDeleteLists(m_chessBoardDL, 1);
    return true;
}

void CGfxOpenGL::AttachToGame(CChessGame *game)
{
    m_chessGame = game;
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
    m_chessGame->Update(dt);

    if (m_chessGame->GetCurrentState() == WAIT_STATE)
        m_currentView = m_chessGame->GetCurrentMoveColor();
}

void CGfxOpenGL::Render()
{
    glClearColor(0.0f, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    if (m_currentView == WHITE)
        gluLookAt(m_whiteViewPos.x, m_whiteViewPos.y, m_whiteViewPos.z, 4.0, 0.0, 4.0, 0.0, 1.0, 0.0);
    else
        gluLookAt(m_blackViewPos.x, m_blackViewPos.y, m_blackViewPos.z, 4.0, 0.0, 4.0, 0.0, 1.0, 0.0);

    // render the wood table
    glDisable(GL_DEPTH_TEST);
    RenderTable();
    glEnable(GL_DEPTH_TEST);
    // prepare to write to the stencil buffer by turning off
    // writes to the color and depth buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    // setup the stencil func and op to place a 1 in the stencil buffer
    // everywhere we're about to draw
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    // render the chess board surface. Since the depth and color buffers are disabled,
    // only the stencil buffer will be modified
    RenderChessBoard();
    // turn color and depth buffers back on
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    // from this point on, only draw where stencil buffer is set to 1
    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
    // don't modify the contents of the stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // draw reflected chess pieces first
    glPushMatrix();
    glScalef(1.0, -1.0, 1.0);
    RenderPieces();
    glPopMatrix();
    // draw chessboard and selection square with blending
    glEnable(GL_BLEND);
    RenderSelections();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glColor4f(0.0, 0.0, 0.0, 0.1f);
    RenderChessBoard();
    glDisable(GL_BLEND);
    // turn off stencil testing
    glDisable(GL_STENCIL_TEST);
    // draw pieces normally
    glPushMatrix();
    glColor4f(1.0, 1.0, 1.0, 1.0);
    RenderPieces();
    glPopMatrix();
}

void CGfxOpenGL::RenderSelections()
{
    for (int x = 0; x < 8; x++) {
        for (int z = 0; z < 8; z++) {
            if (m_chessGame->GetBoardHighlight(x, z)) {
                glColor4f(1.0, 1.0, 0.2, 1.0f); // yellow color
                DrawSelectionBox((float)x, 0.001f, (float)z);
            }
        }
    }
}

void CGfxOpenGL::DrawSelectionBox(float x, float y, float z)
{
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(x, y, z);
    glLineWidth(5.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_POLYGON);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.01, 0.0, 0.01);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.99, 0.0, 0.01);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.99, 0.0, 0.99);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.01, 0.0, 0.99);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1.0f);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

void CGfxOpenGL::RenderTable()
{
    m_texMgr.Bind(m_tableTexture);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glTexCoord2f(10.0, 0.0);
    glVertex3f(20.0f, -0.1f, 20.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-20.0f, -0.1f, 20.0f);
    glTexCoord2f(10.0, 10.0);
    glVertex3f(20.0f, -0.1f, -20.0f);
    glTexCoord2f(0.0, 10.0);
    glVertex3f(-20.0f, -0.1f, -20.0f);
    glEnd();
}

void CGfxOpenGL::GenerateChessBoardDL()
{
    m_chessBoardDL = glGenLists(1);
    glNewList(m_chessBoardDL, GL_COMPILE);

    for (int x = 0; x < 4; x++)
        for (int z = 0; z < 4; z++) {
            for (int row = 2 * x; row < 2 * x + 2; row++) {
                glBegin(GL_TRIANGLE_STRIP);

                for (int col = 2 * z; col < 2 * z + 2; col++) {
                    glTexCoord2f(row / 2.0, col / 2.0);
                    glVertex3f(row, 0.0, col);
                    glTexCoord2f((row + 1) / 2.0, col / 2.0);
                    glVertex3f(row + 1, 0.0, col);
                    glTexCoord2f(row / 2.0, (col + 1) / 2.0);
                    glVertex3f(row, 0.0, col + 1);
                    glTexCoord2f((row + 1) / 2.0, (col + 1) / 2.0);
                    glVertex3f(row + 1, 0.0, col + 1);
                }

                glEnd();
            }
        }

    glEndList();
}

void CGfxOpenGL::RenderChessBoard()
{
    m_texMgr.Bind(m_boardTexture);
    glPushMatrix();
    glCallList(m_chessBoardDL);
    glPopMatrix();
}

void CGfxOpenGL::RenderPieces()
{
    m_chessGame->Render();
}

void CGfxOpenGL::Get3DIntersection(int winx, int winy, double &x, double &y, double &z)
{
    int viewport[4];
    double modelMatrix[16];
    double projMatrix[16];
    double nx, ny, nz;
    double fx, fy, fz;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    gluUnProject((double)winx, (double)(viewport[3] - winy), 0.0, modelMatrix, projMatrix, viewport, &nx, &ny, &nz);
    gluUnProject((double)winx, (double)(viewport[3] - winy), 1.0, modelMatrix, projMatrix, viewport, &fx, &fy, &fz);
    CVector dir = CVector(fx, fy, fz) - CVector(nx, ny, nz);
    CVector yAxisNormal(0.0, 1.0, 0.0);
    CPlane p(yAxisNormal, 0.0);
    double pndotrd = p.N[0] * dir[0] + p.N[1] * dir[1] + p.N[2] * dir[2];
    float point = -(p.N[0] * nx + p.N[1] * ny + p.N[2] * nz + p.D) / pndotrd;
    // get (x, y, z) intersection on (x, z) plane
    x =  nx + (point * dir[0]);
    y =  ny + (point * dir[1]);
    z =  nz + (point * dir[2]);
}

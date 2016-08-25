#ifndef __GL_COMPONENT
#define __GL_COMPONENT

#include "vector.h"

class CMD2Model;
class CChessGame;
class CTextureMgr;


class CGfxOpenGL
{
private:
    int m_windowWidth;
    int m_windowHeight;

    CTextureMgr m_texMgr;

    CChessGame *m_chessGame;

    unsigned int m_chessBoardDL;
    unsigned int m_boardTexture;
    unsigned int m_tableTexture;

    CVector m_whiteViewPos;
    CVector m_blackViewPos;
    char    m_currentView;

    void GenerateChessBoardDL();
    void RenderChessBoard();
    void RenderSelections();
    void RenderPieces();
    void RenderTable();
    void DrawSelectionBox(float x, float y, float z);

public:
    CGfxOpenGL();
    virtual ~CGfxOpenGL();

    bool Init();
    bool Shutdown();

    void AttachToGame(CChessGame *game);

    void SetupProjection(int width, int height);

    void Prepare(float dt);
    void Render();

    void Get3DIntersection(int winx, int winy, double &x, double &y, double &z);
};

#endif
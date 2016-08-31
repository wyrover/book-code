#ifndef __CHESSGAME_H
#define __CHESSGAME_H

// this is the core of the chess game
// rendering, updating each frame, etc. done here

#include "vector.h"
#include "md2.h"
#include "ChessPiece.h"
#include "ChessBoard.h"

// game states
const char WAIT_STATE    = 0x01;    // waiting for input
const char MOVING_STATE  = 0x02;    // moving a piece
const char CAPTURE_STATE = 0x04;    // capturing a piece
const char KILL_STATE    = 0x08;    // killing a piece
const char SWITCH_STATE  = 0x10;    // switching player

class CChessGame
{
private:
    CChessBoard *m_chessBoard;
    ChessPiece  m_pieces[32];

    CMD2Data    m_whitePawn;
    CMD2Data    m_blackPawn;
    CMD2Data    m_whiteRook;
    CMD2Data    m_blackRook;
    CMD2Data    m_whiteKnight;
    CMD2Data    m_blackKnight;
    CMD2Data    m_whiteBishop;
    CMD2Data    m_blackBishop;
    CMD2Data    m_whiteQueen;
    CMD2Data    m_blackQueen;
    CMD2Data    m_whiteKing;
    CMD2Data    m_blackKing;

    int         m_selectedPieceIdx;
    int         m_currentRow;
    int         m_currentCol;

    int         m_pieceMovingIdx;   // current moving piece index
    int         m_pieceCaptureIdx;  // current piece being captured index
    char        m_currentMoveColor; // color with the current move
    char        m_checkColor;       // color of king in check, if any (NO_COLOR is possible)

    char        m_gameState;

    float       m_pieceMoveStartRow;    // starting position of a piece move
    float       m_pieceMoveStartCol;
    float       m_pieceMoveEndRow;      // ending position of a piece move
    float       m_pieceMoveEndCol;
    float       m_pieceMoveCurrentRow;
    float       m_pieceMoveCurrentCol;
    float       m_pieceMoveDistance;    // distance the piece needs to move in world coordinates
    float       m_pieceRotateAngle;

    CVector     m_pieceVelocity;    // velocity vector for movement
    CVector     m_piecePos;         // position vector for movement
    float       m_pieceCaptureDeadTime;
    bool        m_pieceKilled;
    bool        m_pieceDying;

    void SetupWhitePieces();
    void SetupBlackPieces();
    void MovePiece(int pieceIdx, int destRow, int destCol, bool captureMove = false);
    void MovePieceImmediate(int pieceIdx, int destRow, int destCol);
    void CapturePiece(int pieceIdx, int destRow, int destCol);

    bool IsPieceInPlay(int r, int c);
    bool IsPieceCaptured(char color, int r, int c);
    bool IsValidPawnMove(int pieceIdx, int newRow, int newCol);
    bool IsValidRookMove(int pieceIdx, int newRow, int newCol);
    bool IsValidKnightMove(int pieceIdx, int newRow, int newCol);
    bool IsValidBishopMove(int pieceIdx, int newRow, int newCol);
    bool IsValidQueenMove(int pieceIdx, int newRow, int newCol);
    bool IsValidKingMove(int pieceIdx, int newRow, int newCol);
    bool IsCastlingMove(int pieceIdx, int newRow, int newCol);

    void UpdatePieceMovement(float dt);
    void UpdatePieceCapture(float dt);

public:
    CChessGame();
    virtual ~CChessGame();

    void Initialize();
    void Update(float dt);
    void Render();
    void Release();

    CChessBoard *GetBoard()
    {
        return m_chessBoard;
    }
    void OnSelection(float row, float col);

    bool GetBoardHighlight(int row, int col)
    {
        return m_chessBoard->GetHighlight(row, col);
    }
    bool IsValidMove(int pieceIdx, int newRow, int newCol);

    void SetCurrentState(char state)
    {
        m_gameState = state;
    }
    char GetCurrentState()
    {
        return m_gameState;
    }
    char GetCurrentMoveColor()
    {
        return m_currentMoveColor;
    }
};

#endif
#include "ChessBoard.h"

CChessBoard::CChessBoard()
{
}

CChessBoard::~CChessBoard()
{
}

void CChessBoard::Initialize()
{
    ClearBoardPieces();
    int col, row;

    // clear the board
    for (col = 0; col < 8; col++)
        for (row = 0; row < 8; row++) {
            m_highlight[row][col] = false;
        }
}

unsigned char CChessBoard::GetBoardSpace(int row, int col)
{
    if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
        return m_board[row][col];
    else
        return PIECE_NONE;
}

void CChessBoard::SetBoardSpace(int row, int col, unsigned char piece, char color)
{
    if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8))) {
        m_board[row][col] = piece;
    }
}

void CChessBoard::EnableHighlight(int row, int col)
{
    if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
        m_highlight[row][col] = true;
}

void CChessBoard::DisableHighlight(int row, int col)
{
    if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
        m_highlight[row][col] = false;
}

bool CChessBoard::GetHighlight(int row, int col)
{
    if (((row >= 0) && (row < 8)) && ((col >= 0) && (col < 8)))
        return m_highlight[row][col];
    else
        return false;
}

void CChessBoard::ClearBoardPieces()
{
    int col, row;

    // clear the board
    for (col = 0; col < 8; col++)
        for (row = 0; row < 8; row++) {
            m_board[row][col] = PIECE_NONE;
        }
}

void CChessBoard::Release()
{
}
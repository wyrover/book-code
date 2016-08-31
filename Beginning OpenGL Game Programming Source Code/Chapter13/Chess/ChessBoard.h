#ifndef __CHESSBOARD_H
#define __CHESSBOARD_H


const unsigned char PIECE_NONE      = 0x00;
const unsigned char PIECE_PAWN      = 0x01;
const unsigned char PIECE_ROOK      = 0x02;
const unsigned char PIECE_KNIGHT    = 0x04;
const unsigned char PIECE_BISHOP    = 0x10;
const unsigned char PIECE_QUEEN     = 0x20;
const unsigned char PIECE_KING      = 0x40;

const char NO_COLOR                 = 0x00;
const char WHITE                    = 0x01;
const char BLACK                    = 0x02;

class CChessBoard
{
private:
    unsigned char  m_board[8][8];
    bool           m_highlight[8][8];

public:
    CChessBoard();
    virtual ~CChessBoard();

    void Initialize();
    void SetBoardSpace(int row, int col, unsigned char piece, char color);
    unsigned char GetBoardSpace(int row, int col);

    void EnableHighlight(int row, int col);
    void DisableHighlight(int row, int col);
    bool GetHighlight(int row, int col);

    void ClearBoardPieces();

    void Release();
};


#endif
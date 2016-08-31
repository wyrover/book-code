#ifndef __CHESSPIECE_H
#define __CHESSPIECE_H

#include "md2.h"

#define PIECE_IDLE      0
#define PIECE_MOVING    1
#define PIECE_DYING     2

struct ChessPiece {
    char            m_color;
    unsigned char   m_type;
    bool            m_inPlay;
    int             m_row;      // row
    int             m_col;      // col
    char            m_state;    // IDLE, MOVING, DYING
    CMD2Instance   *m_model;
};

#endif
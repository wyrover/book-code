#include "ChessPiece.h"
#include "ChessBoard.h"
#include "ChessGame.h"
#include <iostream>

CChessGame::CChessGame()
{
}

CChessGame::~CChessGame()
{
}

void CChessGame::SetupWhitePieces()
{
    m_whitePawn.Load("hueteotl\\tris.md2", "hueteotl\\hueteotl_white.tga", "hueteotl\\weapon.md2", "hueteotl\\weapon.tga", 0.02f);
    m_whiteRook.Load("rhino\\tris.md2", "rhino\\rhino_white.tga", NULL, NULL, 0.015f);
    m_whiteKnight.Load("pknight\\tris.md2", "pknight\\knight_white.tga", "pknight\\weapon.md2", "pknight\\weapon.tga", 0.025f);
    m_whiteBishop.Load("phantom\\tris.md2", "phantom\\phantom_white.tga", "phantom\\weapon.md2", "phantom\\weapon.tga", 0.03f);
    m_whiteQueen.Load("goblin\\tris.md2", "goblin\\goblin_white.tga", "goblin\\weapon.md2", "goblin\\weapon.tga", 0.03f);
    m_whiteKing.Load("orc\\tris.md2", "orc\\burningblade.tga", "orc\\weapon.md2", "orc\\weapon.tga", 0.03f);

    // setup white pawns
    for (int idx = 0; idx < 8; idx++) {
        m_pieces[idx].m_type = PIECE_PAWN;
        m_pieces[idx].m_col = idx;
        m_pieces[idx].m_row = 1;
        m_pieces[idx].m_color = WHITE;
        m_pieces[idx].m_inPlay = true;
        m_pieces[idx].m_model = m_whitePawn.GetInstance();
    }

    // white rooks
    m_pieces[8].m_type = m_pieces[9].m_type = PIECE_ROOK;
    m_pieces[8].m_color = m_pieces[9].m_color = WHITE;
    m_pieces[8].m_col = 0;
    m_pieces[9].m_col = 7;
    m_pieces[8].m_row = 0;
    m_pieces[9].m_row = 0;
    m_pieces[8].m_inPlay = m_pieces[9].m_inPlay = true;
    m_pieces[8].m_model = m_whiteRook.GetInstance();
    m_pieces[9].m_model = m_whiteRook.GetInstance();
    // white knights
    m_pieces[10].m_type = m_pieces[11].m_type = PIECE_KNIGHT;
    m_pieces[10].m_color = m_pieces[11].m_color = WHITE;
    m_pieces[10].m_col = 1;
    m_pieces[11].m_col = 6;
    m_pieces[10].m_row = 0;
    m_pieces[11].m_row = 0;
    m_pieces[10].m_inPlay = m_pieces[11].m_inPlay = true;
    m_pieces[10].m_model = m_whiteKnight.GetInstance();
    m_pieces[11].m_model = m_whiteKnight.GetInstance();
    // white bishops
    m_pieces[12].m_type = m_pieces[13].m_type = PIECE_BISHOP;
    m_pieces[12].m_color = m_pieces[13].m_color = WHITE;
    m_pieces[12].m_col = 2;
    m_pieces[13].m_col = 5;
    m_pieces[12].m_row = 0;
    m_pieces[13].m_row = 0;
    m_pieces[12].m_inPlay = m_pieces[13].m_inPlay = true;
    m_pieces[12].m_model = m_whiteBishop.GetInstance();
    m_pieces[13].m_model = m_whiteBishop.GetInstance();
    // white queen
    m_pieces[14].m_type = PIECE_QUEEN;
    m_pieces[14].m_color = WHITE;
    m_pieces[14].m_col = 3;
    m_pieces[14].m_row = 0;
    m_pieces[14].m_inPlay = true;
    m_pieces[14].m_model = m_whiteQueen.GetInstance();
    // white king
    m_pieces[15].m_type = PIECE_KING;
    m_pieces[15].m_color = WHITE;
    m_pieces[15].m_col = 4;
    m_pieces[15].m_row = 0;
    m_pieces[15].m_inPlay = true;
    m_pieces[15].m_model = m_whiteKing.GetInstance();
}

void CChessGame::SetupBlackPieces()
{
    m_blackPawn.Load("hueteotl\\tris.md2", "hueteotl\\hueteotl.tga", "hueteotl\\weapon.md2", "hueteotl\\weapon.tga", 0.02f);
    m_blackRook.Load("rhino\\tris.md2", "rhino\\rhino.tga", NULL, NULL, 0.015f);
    m_blackKnight.Load("pknight\\tris.md2", "pknight\\evil.tga", "pknight\\weapon.md2", "pknight\\weapon.tga", 0.025f);
    m_blackBishop.Load("phantom\\tris.md2", "phantom\\phantom.tga", "phantom\\weapon.md2", "phantom\\weapon.tga", 0.03f);
    m_blackQueen.Load("goblin\\tris.md2", "goblin\\goblin.tga", "goblin\\weapon.md2", "goblin\\weapon.tga", 0.03f);
    m_blackKing.Load("orc\\tris.md2", "orc\\blackrock.tga", "orc\\weapon.md2", "orc\\weapon.tga", 0.03f);

    // setup black pawns
    for (int idx = 16; idx < 24; idx++) {
        m_pieces[idx].m_type = PIECE_PAWN;
        m_pieces[idx].m_col = idx - 16;
        m_pieces[idx].m_row = 6;
        m_pieces[idx].m_color = BLACK;
        m_pieces[idx].m_inPlay = true;
        m_pieces[idx].m_model = m_blackPawn.GetInstance();
    }

    // black rooks
    m_pieces[24].m_type = m_pieces[25].m_type = PIECE_ROOK;
    m_pieces[24].m_color = m_pieces[25].m_color = BLACK;
    m_pieces[24].m_col = 0;
    m_pieces[25].m_col = 7;
    m_pieces[24].m_row = 7;
    m_pieces[25].m_row = 7;
    m_pieces[24].m_inPlay = m_pieces[25].m_inPlay = true;
    m_pieces[24].m_model = m_blackRook.GetInstance();
    m_pieces[25].m_model = m_blackRook.GetInstance();
    // black knights
    m_pieces[26].m_type = m_pieces[27].m_type = PIECE_KNIGHT;
    m_pieces[26].m_color = m_pieces[27].m_color = BLACK;
    m_pieces[26].m_col = 1;
    m_pieces[27].m_col = 6;
    m_pieces[26].m_row = 7;
    m_pieces[27].m_row = 7;
    m_pieces[26].m_inPlay = m_pieces[27].m_inPlay = true;
    m_pieces[26].m_model = m_blackKnight.GetInstance();
    m_pieces[27].m_model = m_blackKnight.GetInstance();
    // black bishops
    m_pieces[28].m_type = m_pieces[29].m_type = PIECE_BISHOP;
    m_pieces[28].m_color = m_pieces[29].m_color = BLACK;
    m_pieces[28].m_col = 2;
    m_pieces[29].m_col = 5;
    m_pieces[28].m_row = 7;
    m_pieces[29].m_row = 7;
    m_pieces[28].m_inPlay = m_pieces[29].m_inPlay = true;
    m_pieces[28].m_model = m_blackBishop.GetInstance();
    m_pieces[29].m_model = m_blackBishop.GetInstance();
    // black queen
    m_pieces[30].m_type = PIECE_QUEEN;
    m_pieces[30].m_color = BLACK;
    m_pieces[30].m_col = 3;
    m_pieces[30].m_row = 7;
    m_pieces[30].m_inPlay = true;
    m_pieces[30].m_model = m_blackQueen.GetInstance();
    // black king
    m_pieces[31].m_type = PIECE_KING;
    m_pieces[31].m_color = BLACK;
    m_pieces[31].m_col = 4;
    m_pieces[31].m_row = 7;
    m_pieces[31].m_inPlay = true;
    m_pieces[31].m_model = m_blackKing.GetInstance();
}

void CChessGame::Initialize()
{
    m_chessBoard = new CChessBoard;
    m_chessBoard->Initialize();
    SetupWhitePieces();
    SetupBlackPieces();

    for (int idx = 0; idx < 32; idx++) {
        m_chessBoard->SetBoardSpace(m_pieces[idx].m_row, m_pieces[idx].m_col, m_pieces[idx].m_type, m_pieces[idx].m_color);
    }

    m_selectedPieceIdx = -1;
    m_currentCol       = -1;
    m_currentRow       = -1;
    m_currentMoveColor = WHITE;
    m_checkColor       = NO_COLOR;
    m_gameState        = WAIT_STATE;
}

void CChessGame::Update(float dt)
{
    m_chessBoard->ClearBoardPieces();

    for (int idx = 0; idx < 32; idx++) {
        if (m_pieces[idx].m_inPlay) {
            if (m_pieces[idx].m_model)
                m_pieces[idx].m_model->Animate(dt);

            m_chessBoard->SetBoardSpace(m_pieces[idx].m_row, m_pieces[idx].m_col, m_pieces[idx].m_type, m_pieces[idx].m_color);
        }
    }

    UpdatePieceCapture(dt);
    UpdatePieceMovement(dt);
}

void CChessGame::Render()
{
    for (int idx = 0; idx < 32; idx++) {
        if (m_pieces[idx].m_inPlay) {
            if (m_pieces[idx].m_model) {
                // rotations are done elsewhere during moving and capture states
                if ((m_gameState != MOVING_STATE) && (m_gameState != CAPTURE_STATE) && (m_gameState != KILL_STATE)) {
                    if (m_pieces[idx].m_color == WHITE)
                        m_pieces[idx].m_model->Rotate(0.0f);
                    else if (m_pieces[idx].m_color == BLACK)
                        m_pieces[idx].m_model->Rotate(180.0f);
                }

                // movement is done elsewhere during the MOVING_STATE
                if ((m_gameState != MOVING_STATE) && (m_gameState != KILL_STATE))
                    m_pieces[idx].m_model->Move((float)m_pieces[idx].m_row + 0.5f, 0.0f,
                                                (float)m_pieces[idx].m_col + 0.5f);

                m_pieces[idx].m_model->Render();
            }
        }
    }
}

void CChessGame::Release()
{
    m_chessBoard->Release();

    for (int idx = 0; idx < 32; idx++) {
        if (m_pieces[idx].m_model) {
            m_pieces[idx].m_model->Unload();
            delete m_pieces[idx].m_model;
        }
    }

    delete m_chessBoard;
}

bool CChessGame::IsPieceInPlay(int r, int c)
{
    for (int idx = 0; idx < 32; idx++) {
        if (((m_pieces[idx].m_row == r) && (m_pieces[idx].m_col == c)) &&
            (m_pieces[idx].m_inPlay))
            return true;
    }

    return false;
}

bool CChessGame::IsPieceCaptured(char color, int r, int c)
{
    for (int idx = 0; idx < 32; idx++) {
        if (((m_pieces[idx].m_row == r) && (m_pieces[idx].m_col == c)) &&
            (m_pieces[idx].m_inPlay) && (m_pieces[idx].m_color != color))
            return true;
    }

    return false;
}

void CChessGame::UpdatePieceMovement(float dt)
{
    if (m_gameState == MOVING_STATE) {
        if (m_pieces[m_pieceMovingIdx].m_model->GetAnimation() == CMD2Instance::_STATIC) {
            m_pieces[m_pieceMovingIdx].m_model->Rotate(m_pieceRotateAngle);
            m_pieces[m_pieceMovingIdx].m_model->SetAnimation(CMD2Instance::RUN);
        } else if (m_pieceMoveDistance > 0.75) {
            // haven't reached the end yet
            m_piecePos += m_pieceVelocity * dt;
            m_pieceMoveDistance -= m_pieceVelocity.Length() * dt;
            m_pieceMoveCurrentRow = m_piecePos.x;
            m_pieceMoveCurrentCol = m_piecePos.z;
            m_pieces[m_pieceMovingIdx].m_model->Rotate(m_pieceRotateAngle);
            m_pieces[m_pieceMovingIdx].m_model->Move(m_piecePos.z, 0.0, m_piecePos.x);
        } else if ((m_pieceMoveDistance <= 0.75) && (!m_pieceKilled) && (m_pieceCaptureIdx != -1)) {
            m_gameState = KILL_STATE;
        } else if ((m_pieceMoveDistance <= 0.75) && (m_pieceMoveDistance > 0.0)) {
            // haven't reached the end yet
            m_piecePos += m_pieceVelocity * dt;
            m_pieceMoveDistance -= m_pieceVelocity.Length() * dt;
            m_pieceMoveCurrentRow = m_piecePos.x;
            m_pieceMoveCurrentCol = m_piecePos.z;
            m_pieces[m_pieceMovingIdx].m_model->Rotate(m_pieceRotateAngle);
            m_pieces[m_pieceMovingIdx].m_model->Move(m_piecePos.z, 0.0, m_piecePos.x);
        } else {
            m_pieces[m_pieceMovingIdx].m_col = (int)m_piecePos.x;
            m_pieces[m_pieceMovingIdx].m_row = (int)m_piecePos.z;
            m_pieces[m_pieceMovingIdx].m_model->SetAnimation(CMD2Instance::IDLE);
            // done moving
            m_gameState = WAIT_STATE;
            m_pieceMovingIdx = -1;
        }
    } else if (m_gameState == CAPTURE_STATE) {
        m_pieces[m_pieceMovingIdx].m_model->Rotate(m_pieceRotateAngle);

        if (!m_pieceKilled)
            m_gameState = MOVING_STATE;
    } else if ((m_gameState == KILL_STATE) && (m_pieces[m_pieceMovingIdx].m_model->GetAnimation() == CMD2Instance::_STATIC)) {
        // done with attack animation
        // move forward
        if (m_pieceCaptureDeadTime > 1.5f) {
            m_gameState = MOVING_STATE;
            m_pieceKilled = true;
        }
    } else if (m_gameState == KILL_STATE) {
        m_pieces[m_pieceMovingIdx].m_model->SetAnimation(CMD2Instance::ATTACK, CMD2Instance::_STATIC);
    }
}

void CChessGame::UpdatePieceCapture(float dt)
{
    if (/*(m_gameState == CAPTURE_STATE) || */(m_gameState == KILL_STATE)) {
        if (m_pieces[m_pieceCaptureIdx].m_model->GetAnimation() == CMD2Instance::WAVE) //&&
            //(m_pieces[m_pieceCaptureIdx].m_model->GetAnimation() != CMD2Instance::_STATIC))
        {
            if (m_pieceMoveDistance <= 0.75f) {
                // flipoff once more
                m_pieces[m_pieceCaptureIdx].m_model->SetAnimation(CMD2Instance::WAVE, CMD2Instance::_STATIC);
                m_pieceDying = false;
            }
        }

        if (m_pieces[m_pieceCaptureIdx].m_model->GetAnimation() == CMD2Instance::_STATIC) {
            // stopped last flipoff, time to die
            if ((m_pieces[m_pieceMovingIdx].m_model->GetAnimation() == CMD2Instance::_STATIC) && (!m_pieceDying)) {
                // set animation to death and have it stop on the last frame
                m_pieces[m_pieceCaptureIdx].m_model->SetAnimation(CMD2Instance::DEATH1, CMD2Instance::_STATIC);
                m_pieceDying = true;
                m_pieceCaptureDeadTime = 0.0f;
            }

            m_pieceCaptureDeadTime += dt;

            if (m_pieceCaptureDeadTime >= 1.5f) {
                m_pieceCaptureDeadTime = 0.0f;
                m_pieces[m_pieceCaptureIdx].m_inPlay = false;
                m_gameState = MOVING_STATE;
                m_pieceCaptureIdx = -1;
            }
        }
    } else if (m_gameState == KILL_STATE) {
    }
}

void CChessGame::MovePiece(int pieceIdx, int destRow, int destCol, bool captureMove)
{
    m_pieceMoveStartRow = m_pieces[pieceIdx].m_row + 0.5f;
    m_pieceMoveStartCol = m_pieces[pieceIdx].m_col + 0.5f;
    m_pieceMoveEndRow   = destRow + 0.5f;
    m_pieceMoveEndCol   = destCol + 0.5f;
    m_pieceMoveCurrentRow = m_pieceMoveStartRow;
    m_pieceMoveCurrentCol = m_pieceMoveStartCol;
    m_piecePos = CVector(m_pieceMoveStartCol, 0.0, m_pieceMoveStartRow);
    m_pieceVelocity = CVector(m_pieceMoveEndCol - m_pieceMoveStartCol, 0.0, m_pieceMoveEndRow - m_pieceMoveStartRow);
    m_pieceMoveDistance = m_pieceVelocity.Length();
    m_pieceVelocity.Normalize();

    if (m_pieceVelocity.z > 0)
        m_pieceRotateAngle = RAD2DEG(m_pieceVelocity.Angle(CVector(1.0, 0.0, 0.0))) - 90.0f;
    else
        m_pieceRotateAngle = RAD2DEG(m_pieceVelocity.Angle(CVector(-1.0, 0.0, 0.0))) + 90.0f;

    m_pieceMovingIdx = pieceIdx;
    m_pieceKilled = false;
    m_pieceCaptureIdx = -1;

    if ((m_gameState == CAPTURE_STATE) || (m_gameState == MOVING_STATE)) {
        m_pieces[pieceIdx].m_model->SetAnimation(CMD2Instance::RUN);
    }
}

void CChessGame::MovePieceImmediate(int pieceIdx, int destRow, int destCol)
{
    m_pieces[pieceIdx].m_row = destRow;
    m_pieces[pieceIdx].m_col = destCol;
}

void CChessGame::CapturePiece(int pieceIdx, int destRow, int destCol)
{
    m_gameState = CAPTURE_STATE;
    m_pieceCaptureIdx = pieceIdx;
    m_pieces[pieceIdx].m_model->SetAnimation(CMD2Instance::WAVE);
}

bool CChessGame::IsValidPawnMove(int pieceIdx, int newRow, int newCol)
{
    // same position
    if ((newRow == m_pieces[pieceIdx].m_row) && (newCol == m_pieces[pieceIdx].m_col))
        return false;

    if (m_pieces[pieceIdx].m_color == WHITE) {
        // can only increase row, or rank, of pawns by one
        // unless it's current row/rank is 1 (starts at 0)
        if ((newRow == (m_pieces[pieceIdx].m_row + 1)) && (newCol == m_pieces[pieceIdx].m_col)) {
            // make sure no pieces are already in this location
            if (m_chessBoard->GetBoardSpace(newRow, newCol) == PIECE_NONE)
                return (!IsPieceInPlay(newRow, newCol));
        } else if ((newRow == (m_pieces[pieceIdx].m_row + 2)) && (m_pieces[pieceIdx].m_row == 1) && (newCol == m_pieces[pieceIdx].m_col)) {
            // make sure no pieces are already in this location
            // make sure no pieces are already in this location
            if (m_chessBoard->GetBoardSpace(newRow, newCol) == PIECE_NONE)
                return (!IsPieceInPlay(newRow, newCol));
        } else if (((newRow == (m_pieces[pieceIdx].m_row + 1)) && (newCol == m_pieces[pieceIdx].m_col + 1)) ||
                   ((newRow == (m_pieces[pieceIdx].m_row + 1)) && (newCol == m_pieces[pieceIdx].m_col - 1))) {
            // capture move
            if (m_chessBoard->GetBoardSpace(newRow, newCol) == PIECE_NONE)
                return false;
            else    // a piece is there
                return IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol);
        } else
            return false;
    } else if (m_pieces[pieceIdx].m_color == BLACK) {
        // can only increase row, or rank, of pawns by one
        // unless it's current row/rank is 1 (starts at 0)
        if ((newRow == (m_pieces[pieceIdx].m_row - 1)) && (newCol == m_pieces[pieceIdx].m_col)) {
            if (m_chessBoard->GetBoardSpace(newRow, newCol) == PIECE_NONE)
                return (!IsPieceInPlay(newRow, newCol));
        } else if ((newRow == (m_pieces[pieceIdx].m_row - 2)) && (m_pieces[pieceIdx].m_row == 6) && (newCol == m_pieces[pieceIdx].m_col)) {
            if (m_chessBoard->GetBoardSpace(newRow, newCol) == PIECE_NONE)
                return (!IsPieceInPlay(newRow, newCol));
        } else if (((newRow == (m_pieces[pieceIdx].m_row - 1)) && (newCol == m_pieces[pieceIdx].m_col + 1)) ||
                   ((newRow == (m_pieces[pieceIdx].m_row - 1)) && (newCol == m_pieces[pieceIdx].m_col - 1))) {
            // capture move
            if (m_chessBoard->GetBoardSpace(newRow, newCol) == PIECE_NONE)
                return false;
            else    // a piece is there
                return IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol);
        } else
            return false;
    }

    return false;
}

bool CChessGame::IsValidRookMove(int pieceIdx, int newRow, int newCol)
{
    // same position
    if ((newRow == m_pieces[pieceIdx].m_row) && (newCol == m_pieces[pieceIdx].m_col))
        return false;

    // moving along same row
    if ((newRow == (m_pieces[pieceIdx].m_row)) && (newCol != m_pieces[pieceIdx].m_col)) {
        // iterate along spaces between current position and new position, and see if there are any pieces
        if ((newCol - m_pieces[pieceIdx].m_col) > 0) {      // move in positive direction
            for (int c = m_pieces[pieceIdx].m_col + 1; c <= newCol; c++) {
                if (m_chessBoard->GetBoardSpace(m_pieces[pieceIdx].m_row, c) != PIECE_NONE) {
                    if (c == newCol) {  // reached end of iteration, see if piece capture
                        if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                            return true;
                    }

                    return (!IsPieceInPlay(m_pieces[pieceIdx].m_row, c));
                }
            }

            // never found any pieces in the way
            return true;
        } else if ((newCol - m_pieces[pieceIdx].m_col) < 0) { // move in negative direction
            for (int c = m_pieces[pieceIdx].m_col - 1; c >= newCol; c--) {
                if (m_chessBoard->GetBoardSpace(m_pieces[pieceIdx].m_row, c) != PIECE_NONE) {
                    if (c == newCol) {  // reached end of iteration, see if piece capture
                        if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                            return true;
                    }

                    return (!IsPieceInPlay(m_pieces[pieceIdx].m_row, c));
                }
            }

            // never found any pieces obstructing the move
            return true;
        }

        // selected the same square, not a move
        return false;
    }
    // moving along same column
    else if ((newRow != (m_pieces[pieceIdx].m_row)) && (newCol == m_pieces[pieceIdx].m_col)) {
        // iterate along spaces between current position and new position, and see if there are any pieces
        if ((newRow - m_pieces[pieceIdx].m_row) > 0) {      // move in positive direction
            for (int r = m_pieces[pieceIdx].m_row + 1; r <= newRow; r++) {
                if ((m_chessBoard->GetBoardSpace(r, m_pieces[pieceIdx].m_col) != PIECE_NONE)) {
                    if (r == newRow) {  // reached end of iteration, see if piece capture
                        if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                            return true;
                    }

                    return (!IsPieceInPlay(r, m_pieces[pieceIdx].m_col));
                }
            }

            // never found any pieces in the way
            return true;
        } else if ((newRow - m_pieces[pieceIdx].m_row) < 0) { // move in negative direction
            for (int r = m_pieces[pieceIdx].m_row - 1; r >= newRow; r--) {
                if ((m_chessBoard->GetBoardSpace(r, m_pieces[pieceIdx].m_col) != PIECE_NONE)) {
                    if (r == newRow) {  // reached end of iteration, see if piece capture
                        if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                            return true;
                    }

                    return (!IsPieceInPlay(r, m_pieces[pieceIdx].m_col));
                }
            }

            // never found any pieces obstructing the move
            return true;
        }

        // selected the same square, not a move
        return false;
    }

    // any other direction is invalid
    return false;
}

bool CChessGame::IsValidKnightMove(int pieceIdx, int newRow, int newCol)
{
    // same position
    if ((newRow == m_pieces[pieceIdx].m_row) && (newCol == m_pieces[pieceIdx].m_col))
        return false;

    // knights have 8 possible moves, provided all these moves are within the bounds of the board
    // the checks for being out of bounds have already been executed prior to this function,
    // so we can just check for the normal 8 knight moves

    /*
     Layout of knight moves - X is the current position
     .2.1.
     3...8
     ..X..
     4...7
     .5.6.
     */

    // move 1
    if ((m_pieces[pieceIdx].m_row == newRow - 2) && (m_pieces[pieceIdx].m_col == newCol + 1)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 2
    else if ((m_pieces[pieceIdx].m_row == newRow - 2) && (m_pieces[pieceIdx].m_col == newCol - 1)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 3
    else if ((m_pieces[pieceIdx].m_row == newRow - 1) && (m_pieces[pieceIdx].m_col == newCol - 2)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 4
    else if ((m_pieces[pieceIdx].m_row == newRow + 1) && (m_pieces[pieceIdx].m_col == newCol - 2)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 5
    else if ((m_pieces[pieceIdx].m_row == newRow + 2) && (m_pieces[pieceIdx].m_col == newCol - 1)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 6
    else if ((m_pieces[pieceIdx].m_row == newRow + 2) && (m_pieces[pieceIdx].m_col == newCol + 1)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 7
    else if ((m_pieces[pieceIdx].m_row == newRow + 1) && (m_pieces[pieceIdx].m_col == newCol + 2)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }
    // move 8
    else if ((m_pieces[pieceIdx].m_row == newRow - 1) && (m_pieces[pieceIdx].m_col == newCol + 2)) {
        return (!IsPieceInPlay(newRow, newCol) || IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol));
    }

    return false;
}

bool CChessGame::IsValidBishopMove(int pieceIdx, int newRow, int newCol)
{
    int r;
    int c;

    // same position
    if ((newRow == m_pieces[pieceIdx].m_row) && (newCol == m_pieces[pieceIdx].m_col))
        return false;

    // first verify that the new position is on a diagonal
    // for \ diagonal
    // if the sum of the row and col in the new position is the same
    // as the row and col of the current position then the move is diagonal

    if ((m_pieces[pieceIdx].m_row <= newRow) && (m_pieces[pieceIdx].m_col <= newCol)) {
        // move up,left  \ diagonal
        if (!((m_pieces[pieceIdx].m_row + newCol) == (newRow + m_pieces[pieceIdx].m_col)))
            return false;

        r = m_pieces[pieceIdx].m_row + 1;
        c = m_pieces[pieceIdx].m_col + 1;

        // check for obstructing pieces
        while ((r <= newRow) && (c <= newCol)) {
            if ((r == newRow) && (c == newCol))
                if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                    return true;

            if (IsPieceInPlay(r, c))
                return false;

            r++;
            c++;
        }
    } else if ((m_pieces[pieceIdx].m_row <= newRow) && (m_pieces[pieceIdx].m_col >= newCol)) {
        // move up, right \ diagonal
        if (!((m_pieces[pieceIdx].m_row + m_pieces[pieceIdx].m_col) == (newRow + newCol)))
            return false;

        r = m_pieces[pieceIdx].m_row + 1;
        c = m_pieces[pieceIdx].m_col - 1;

        // check for obstructing pieces
        while ((r <= newRow) && (c >= newCol)) {
            if ((r == newRow) && (c == newCol))
                if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                    return true;

            if (IsPieceInPlay(r, c))
                return false;

            r++;
            c--;
        }
    } else if ((m_pieces[pieceIdx].m_row >= newRow) && (m_pieces[pieceIdx].m_col >= newCol)) {
        // swap columns for / diagonal
        // move down, right / diagonal
        if (!((m_pieces[pieceIdx].m_row + newCol) == (newRow + m_pieces[pieceIdx].m_col)))
            return false;

        r = m_pieces[pieceIdx].m_row - 1;
        c = m_pieces[pieceIdx].m_col - 1;

        // check for obstructing pieces
        while ((r >= newRow) && (c >= newCol)) {
            if ((r == newRow) && (c == newCol))
                if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                    return true;

            if (IsPieceInPlay(r, c))
                return false;

            r--;
            c--;
        }
    } else if ((m_pieces[pieceIdx].m_row >= newRow) && (m_pieces[pieceIdx].m_col <= newCol)) {
        // swap columns for / diagonal
        // move down, left / diagonal
        if (!((m_pieces[pieceIdx].m_row + m_pieces[pieceIdx].m_col) == (newRow + newCol)))
            return false;

        r = m_pieces[pieceIdx].m_row - 1;
        c = m_pieces[pieceIdx].m_col + 1;

        // check for obstructing pieces
        while ((r >= newRow) && (c <= newCol)) {
            if ((r == newRow) && (c == newCol))
                if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
                    return true;

            if (IsPieceInPlay(r, c))
                return false;

            r--;
            c++;
        }
    } else
        return false;

    //if (abs(m_pieces[pieceIdx].m_row - newRow) == abs(m_pieces[pieceIdx].m_col - newCol))
    //{
//      return true;
//  }
//  else
//      return false;
    return true;
}

bool CChessGame::IsValidQueenMove(int pieceIdx, int newRow, int newCol)
{
    // same position
    if ((newRow == m_pieces[pieceIdx].m_row) && (newCol == m_pieces[pieceIdx].m_col))
        return false;

    if (IsValidRookMove(pieceIdx, newRow, newCol) || IsValidBishopMove(pieceIdx, newRow, newCol))
        return true;

    return false;
}

bool CChessGame::IsCastlingMove(int pieceIdx, int newRow, int newCol)
{
    // may not castle if:
    // 1. Your king has been moved earlier in the game.
    // 2. The rook that castles has been moved earlier in the game.
    // 3. There are pieces standing between your king and rook.
    // 4. The king is in check.
    // 5. The king moves through a square that is attacked by a piece of the opponent.
    // 6. The king would be in check after castling.

    // check for condition #1
    if ((m_pieces[pieceIdx].m_color == WHITE) && (m_pieces[pieceIdx].m_row == 0) &&
        (m_pieces[pieceIdx].m_col == 4)) {
        // check for condition #3 and #5
        // row 0
        if (newCol == 2) { // queenside
            if (IsPieceInPlay(0, 3) || IsPieceInPlay(0, 2) || IsPieceInPlay(0, 1))
                return false;
        } else if (newCol == 6) { // kingside
            if (IsPieceInPlay(0, 5) || IsPieceInPlay(0, 6))
                return false;
        }

        return true;
    } else if (m_pieces[pieceIdx].m_color == BLACK) {
        // check for condition #3 and #5
        // row 7
        if (newCol == 2) { // queenside
            if (IsPieceInPlay(7, 3) || IsPieceInPlay(7, 2) || IsPieceInPlay(7, 1))
                return false;
        } else if (newCol == 6) { // kingside
            if (IsPieceInPlay(7, 5) || IsPieceInPlay(7, 6))
                return false;
        }
    }

    return true;
}

bool CChessGame::IsValidKingMove(int pieceIdx, int newRow, int newCol)
{
    // same position
    if ((newRow == m_pieces[pieceIdx].m_row) && (newCol == m_pieces[pieceIdx].m_col))
        return false;

    if (IsCastlingMove(pieceIdx, newRow, newCol))
        return true;

    // kings may move only one space at a time, in any direction
    if ((abs(newRow - m_pieces[pieceIdx].m_row) > 1) || (abs(newCol - m_pieces[pieceIdx].m_col) > 1))
        return false;

    if (IsPieceCaptured(m_pieces[pieceIdx].m_color, newRow, newCol))
        return true;

    if (IsPieceInPlay(newRow, newCol))
        return false;

    return true;
}

bool CChessGame::IsValidMove(int pieceIdx, int newRow, int newCol)
{
    if (m_pieces[pieceIdx].m_color == m_currentMoveColor) {
        switch (m_pieces[pieceIdx].m_type) {
        case PIECE_PAWN:
            return IsValidPawnMove(pieceIdx, newRow, newCol);

        case PIECE_ROOK:
            return IsValidRookMove(pieceIdx, newRow, newCol);

        case PIECE_KNIGHT:
            return IsValidKnightMove(pieceIdx, newRow, newCol);

        case PIECE_BISHOP:
            return IsValidBishopMove(pieceIdx, newRow, newCol);

        case PIECE_QUEEN:
            return IsValidQueenMove(pieceIdx, newRow, newCol);

        case PIECE_KING:
            return IsValidKingMove(pieceIdx, newRow, newCol);
        }
    }

    return false;
}

void CChessGame::OnSelection(float col, float row)
{
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
            m_chessBoard->DisableHighlight(x, y);

    if (m_gameState == WAIT_STATE) {
        if (((col <= 8.0) && (col >= 0.0)) && ((row >= 0.0) && (row <= 8.0))) {
            if (m_selectedPieceIdx != -1) {
                bool pieceCapture = false;

                // see if a piece of the opposite color is at the new position. if so, then capture
                for (int idx = 0; idx < 32; idx++) {
                    if ((m_pieces[idx].m_row == (int)row) && (m_pieces[idx].m_col == (int)col) &&
                        m_pieces[idx].m_inPlay && (m_pieces[m_selectedPieceIdx].m_color != m_pieces[idx].m_color)) {
                        // do we have a capture?
                        if ((IsPieceCaptured(m_currentMoveColor, (int)row, (int)col)) && (IsValidMove(m_selectedPieceIdx, (int)row, (int)col))) {
                            m_gameState = CAPTURE_STATE;
                            MovePiece(m_selectedPieceIdx, (int)row, (int)col, true);
                            CapturePiece(idx, (int)row, (int)col);
                            pieceCapture = true;

                            // we've moved, so now it's the other color's turn to move
                            if (m_currentMoveColor == WHITE)
                                m_currentMoveColor = BLACK;
                            else if (m_currentMoveColor == BLACK)
                                m_currentMoveColor = WHITE;

                            break;
                        }
                    }
                }

                // we already have a piece selected, so move it if possible
                if (!pieceCapture && IsValidMove(m_selectedPieceIdx, (int)row, (int)col)) {
                    m_gameState = MOVING_STATE;

                    // check if castling
                    if (IsCastlingMove(m_selectedPieceIdx, (int)row, (int)col)) {
                        // move rooks
                        if (((int)row == 0) && ((int)col == 6))
                            MovePieceImmediate(9, 0, 5);
                        else if (((int)row == 0) && ((int)col == 2))
                            MovePieceImmediate(8, 0, 3);
                        else if (((int)row == 7) && ((int)col == 6))
                            MovePieceImmediate(25, 7, 5);
                        else if (((int)row == 7) && ((int)col == 2))
                            MovePieceImmediate(24, 7, 3);
                    }

                    MovePiece(m_selectedPieceIdx, (int)row, (int)col);

                    // we've moved, so now it's the other color's turn to move
                    if (m_currentMoveColor == WHITE)
                        m_currentMoveColor = BLACK;
                    else if (m_currentMoveColor == BLACK)
                        m_currentMoveColor = WHITE;
                }

                m_selectedPieceIdx = -1;
                m_currentRow = -1;
                m_currentCol = -1;
            } else {
                // no pieces currently selected, so we select a square
                m_chessBoard->EnableHighlight((int)row, (int)col);

                // check through pieces for this location, if a piece is there, select the piece
                for (int idx = 0; idx < 32; idx++) {
                    if ((m_pieces[idx].m_row == (int)row) && (m_pieces[idx].m_col == (int)col) && (m_pieces[idx].m_inPlay)) {
                        m_selectedPieceIdx = idx;
                        break;
                    }
                }

                m_currentRow = (int)row;
                m_currentCol = (int)col;
            }
        } else {
            m_selectedPieceIdx = -1;
            m_currentRow = -1;
            m_currentCol = -1;
        }
    }
}
#include <iostream>
#include "MyPair.hpp"
#include "MyVector.hpp"
#pragma once

// search difficulty to noise, pool width
static constexpr int NOISE_BEGINNER = 50;
static constexpr int MARGIN_BEGINNER = 200;
static constexpr int NOISE_EASY = 25;
static constexpr int MARGIN_EASY = 100;
static constexpr int NOISE_MEDIUM = 10;
static constexpr int MARGIN_MEDIUM = 50;
static constexpr int NOISE_HARD = 0;
static constexpr int MARGIN_HARD = 0;

// seed-generation bit-magic 
static constexpr unsigned OBJ_HIGH_SHIFT = 32U;
static constexpr unsigned VAR_HIGH_SHIFT = 32U;
static constexpr unsigned MIX_LEFT_SHIFT = 16U;
static constexpr unsigned MIX_RIGHT_SHIFT = 3U;
static constexpr unsigned XOR_CONST = 0xA5A5U;
static constexpr unsigned XORSHIFT_L = 13U;
static constexpr unsigned XORSHIFT_R = 17U;

// board geometry
static constexpr int ROWS_COUNT = 8;
static constexpr int COLS_COUNT = 8;
static constexpr int BOARD_SIZE = 8;
static constexpr int8_t INVALID_ROW = -1;
static constexpr int8_t INVALID_COL = -1;

// king, rook start and castle squares
static constexpr int8_t KING_START_COL = 4;
static constexpr int8_t KS_VIA_COL = 5;
static constexpr int8_t QS_VIA_COL = 3;
static constexpr int8_t KS_ROOK_FROM_COL = 7;
static constexpr int8_t KS_ROOK_TO_COL = 5;
static constexpr int8_t QS_ROOK_FROM_COL = 0;
static constexpr int8_t QS_ROOK_TO_COL = 3;

// en-passant target rows and directions
static constexpr int8_t EP_ROW_WHITE = 3;
static constexpr int8_t EP_ROW_BLACK = 4;
static constexpr int8_t EP_DIR_WHITE = -1;
static constexpr int8_t EP_DIR_BLACK = +1;

// starting, promo ranks
static constexpr int WHITE_BACK_ROW = BOARD_SIZE - 1;
static constexpr int WHITE_PAWN_ROW = BOARD_SIZE - 2;
static constexpr int BLACK_BACK_ROW = 0;
static constexpr int BLACK_PAWN_ROW = 1;
static constexpr int WHITE_PROMOTE_ROW = 0;
static constexpr int BLACK_PROMOTE_ROW = BOARD_SIZE - 1;

// empty squares that must be clear for castling
static constexpr int KS_EMPTY_COL1 = 5;
static constexpr int KS_EMPTY_COL2 = 6;
static constexpr int QS_EMPTY_COL1 = 1;
static constexpr int QS_EMPTY_COL2 = 2;
static constexpr int QS_EMPTY_COL3 = 3;

// alpha-beta initial window
static constexpr int ALPHA_INIT = -1000000;
static constexpr int BETA_INIT = 1000000;

// piece material values (centipawns)
static constexpr int VALUE_PAWN = 100;
static constexpr int VALUE_KNIGHT = 320;
static constexpr int VALUE_BISHOP = 330;
static constexpr int VALUE_ROOK = 500;
static constexpr int VALUE_QUEEN = 900;

// absolute score bounds
static constexpr int MIN_SCORE = -100000;
static constexpr int MAX_SCORE = 100000;
static constexpr int DEFAULT_DEPTH = 3;

// king two-square shift when castling
static constexpr int KING_CASTLE_SHIFT = 2;


enum class MyColor: uint8_t
{

    WHITE,
    BLACK

};

enum class FigureType: uint8_t
{

    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING

};

// back-rank starting lineup (a-h)
static constexpr FigureType backrank[BOARD_SIZE] =
{

    FigureType::ROOK, FigureType::KNIGHT, FigureType::BISHOP,
    FigureType::QUEEN, FigureType::KING, FigureType::BISHOP,
    FigureType::KNIGHT, FigureType::ROOK

};

enum class SpecialMove: uint8_t
{

    NORMAL,
    CASTLING_KING_SIDE,
    CASTLING_QUEEN_SIDE,
    EN_PASSANT,
    PROMOTION,
    DOUBLE_PAWN

};

enum class Piece: uint8_t
{

    NONE,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK

};

enum class Difficulty: uint8_t
{

    Beginner = 1,
    Easy = 2, 
    Medium = 3, 
    Hard = 5 

};

static MyVector<MyPair<int, int>> makeDiagonalDirs() // 4 diag
{

    MyVector<MyPair<int, int>> dirs;
    dirs.push_back({ -1, -1 });
    dirs.push_back({ -1, +1 });
    dirs.push_back({ +1, -1 });
    dirs.push_back({ +1, +1 });

    return dirs;

}

static MyVector<MyPair<int, int>> makeOrthogonalDirs() // 4 straight
{

    MyVector<MyPair<int, int>> dirs;
    dirs.push_back({ -1,  0 });
    dirs.push_back({ +1,  0 });
    dirs.push_back({ 0, -1 });
    dirs.push_back({ 0, +1 });

    return dirs;

}

static MyVector<MyPair<int, int>> makeKingDirs() // 8 king steps
{

    MyVector<MyPair<int, int>> dirs;

    dirs.push_back({ +1,  0 });
    dirs.push_back({ +1, +1 });
    dirs.push_back({ 0, +1 });
    dirs.push_back({ -1, +1 });
    dirs.push_back({ -1,  0 });
    dirs.push_back({ -1, -1 });
    dirs.push_back({ 0, -1 });
    dirs.push_back({ +1, -1 });

    return dirs;

}

static MyVector<MyPair<int, int>> makeKnightDirs() // 8 L-jumps
{

    MyVector<MyPair<int, int>> dirs;

    dirs.push_back({ -2, -1 });
    dirs.push_back({ -2, +1 });
    dirs.push_back({ -1, -2 });
    dirs.push_back({ -1, +2 });
    dirs.push_back({ +1, -2 });
    dirs.push_back({ +1, +2 });
    dirs.push_back({ +2, -1 });
    dirs.push_back({ +2, +1 });

    return dirs;

}

static MyVector<MyPair<int, int>> makeQueenDirs() // 8 dirs
{

    MyVector<MyPair<int, int>> dirs = makeOrthogonalDirs();
    MyVector<MyPair<int, int>> iter = makeDiagonalDirs();

    for (size_t i = 0; i < iter.size(); i++)
    {

        dirs.push_back(iter[i]);

    }

    return dirs;

}

static MyVector<MyPair<int, int>> makeRookDirs() // 4 dirs
{

    MyVector<MyPair<int, int>> dirs;

    dirs.push_back(MyPair<int, int>(-1, 0));
    dirs.push_back(MyPair<int, int>(1, 0));
    dirs.push_back(MyPair<int, int>(0, -1));
    dirs.push_back(MyPair<int, int>(0, 1));

    return dirs;

}

// ready-made constant vectors for move generation
static const MyVector<MyPair<int, int>> DIAGONAL_DIRS = makeDiagonalDirs();
static const MyVector<MyPair<int, int>> ORTHOGONAL_DIRS = makeOrthogonalDirs();
static const MyVector<MyPair<int, int>> KING_DIRS = makeKingDirs();
static const MyVector<MyPair<int, int>> KNIGHT_DIRS = makeKnightDirs();
static const MyVector<MyPair<int, int>> QUEEN_DIRS = makeQueenDirs();
static const MyVector<MyPair<int, int>> ROOK_DIRS = makeRookDirs();

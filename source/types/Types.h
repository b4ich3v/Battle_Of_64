#include <iostream>
#pragma once

static constexpr int ROWS_COUNT = 8;
static constexpr int COLS_COUNT = 8;
static constexpr int BOARD_SIZE = 8;

static constexpr int8_t INVALID_ROW = -1;
static constexpr int8_t INVALID_COL = -1;

static constexpr int8_t KING_START_COL = 4;

static constexpr int8_t KS_VIA_COL = 5;
static constexpr int8_t QS_VIA_COL = 3;

static constexpr int8_t KS_ROOK_FROM_COL = 7;
static constexpr int8_t KS_ROOK_TO_COL = 5;
static constexpr int8_t QS_ROOK_FROM_COL = 0;
static constexpr int8_t QS_ROOK_TO_COL = 3;

static constexpr int8_t EP_ROW_WHITE = 3;
static constexpr int8_t EP_ROW_BLACK = 4;

static constexpr int8_t EP_DIR_WHITE = -1;
static constexpr int8_t EP_DIR_BLACK = +1;

static constexpr int WHITE_BACK_ROW = BOARD_SIZE - 1;
static constexpr int WHITE_PAWN_ROW = BOARD_SIZE - 2;
static constexpr int BLACK_BACK_ROW = 0;
static constexpr int BLACK_PAWN_ROW = 1;

static constexpr int KS_EMPTY_COL1 = 5; 
static constexpr int KS_EMPTY_COL2 = 6; 

static constexpr int QS_EMPTY_COL1 = 1; 
static constexpr int QS_EMPTY_COL2 = 2; 
static constexpr int QS_EMPTY_COL3 = 3;

static constexpr int ALPHA_INIT = -1000000;
static constexpr int BETA_INIT = 1000000;

static constexpr int VALUE_PAWN = 100;
static constexpr int VALUE_KNIGHT = 320;
static constexpr int VALUE_BISHOP = 330;
static constexpr int VALUE_ROOK = 500;
static constexpr int VALUE_QUEEN = 900;

static constexpr int MIN_SCORE = -100000;
static constexpr int MAX_SCORE = 100000;
static constexpr int DEFAULT_DEPTH = 3;

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

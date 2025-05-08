#include "Figure.h"
#include "Position.h"
#pragma once

enum class SpecialMove: uint8_t
{

    NORMAL,
    CASTLING_KING_SIDE,
    CASTLING_QUEEN_SIDE,
    EN_PASSANT,
    PROMOTION,
    DOUBLE_PAWN

};

struct Move
{
public:

    Position from;
    Position to;

    SpecialMove special;
    FigureType promotionType;

    Move();

    Move(Position f, Position t,
        SpecialMove s = SpecialMove::NORMAL,FigureType p = FigureType::QUEEN);

    void execute(class Board& board) const;

    void undo(class Board& board) const;

};


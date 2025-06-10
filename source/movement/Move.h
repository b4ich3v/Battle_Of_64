#include "Figure.h"
#include "Position.h"
#pragma once

struct Move 
{
public:

    Position from;
    Position to;

    SpecialMove special;
    FigureType promotionType;

    Move();

    Move(Position from, Position to, 
        SpecialMove specialMove = SpecialMove::NORMAL, FigureType type = FigureType::QUEEN);

    SpecialMove getSpecial() const;

    void execute(class Board& board) const;

    void undo(class Board& board) const;

};


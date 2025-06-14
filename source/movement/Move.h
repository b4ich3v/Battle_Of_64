#include "Figure.h"
#include "Position.h"
#pragma once

struct Move 
{
public:

    Position from; // origin square
    Position to; // destination square

    SpecialMove special; // NORMAL, CASTLE, EP and so on
    FigureType promotionType; // target piece if PROMOTION

    Move(); // default 0 to 0

    Move(Position from, Position to, 
        SpecialMove specialMove = SpecialMove::NORMAL, FigureType type = FigureType::QUEEN);

    SpecialMove getSpecial() const;

    void execute(class Board& board) const; // apply on board and history

    void undo(class Board& board) const; // revert via history

};


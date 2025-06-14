#include "SlidingFigure.h"
#pragma once

class Bishop: public SlidingFigure 
{
public:

    explicit Bishop(MyColor color); // ctor to color only

    MyVector<Move> generateMoves(const Board& board, const Position& from) const override; // DIAGONAL_DIRS

    void accept(Visitor& visitor) const override; // visitor pattern

    char symbol() const override; // 'B', 'b'

};


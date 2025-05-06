#include "Figure.h"
#pragma once

class Pawn: public Figure
{
public:

    explicit Pawn(Color color);

    MyVector<Position> generateMoves(Board const& board, Position const& from) const override;

    char symbol() const override;

};



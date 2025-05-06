#include "Figure.h"
#pragma once

class Knight: public Figure
{
public:

    explicit Knight(Color color);

    MyVector<Position> generateMoves(const Board& board, const Position& from) const override;

    char symbol() const override;

};


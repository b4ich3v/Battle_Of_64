#include "SlidingFigure.h"
#pragma once

class Rook: public SlidingFigure 
{
public:

    explicit Rook(Color color);

    MyVector<Position> generateMoves(const Board& board, const Position& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};


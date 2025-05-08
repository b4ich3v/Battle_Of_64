#include "SlidingFigure.h"
#pragma once

class Bishop: public SlidingFigure 
{
public:

    explicit Bishop(Color color);

    MyVector<Position> generateMoves(const Board& board, const Position& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};


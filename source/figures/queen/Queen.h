#include "SlidingFigure.h"
#pragma once

class Queen: public SlidingFigure 
{
public:

    explicit Queen(Color color);

    MyVector<Position> generateMoves(const Board& board,const Position& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};


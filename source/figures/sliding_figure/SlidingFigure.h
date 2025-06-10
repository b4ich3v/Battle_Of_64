#include "Figure.h"
#pragma once

class SlidingFigure: public Figure
{
protected:
 
    MyVector<Move>generateSliding(const Board& board, const Position& from,
            const MyVector<MyPair<int, int>>& directions) const;

public:

    SlidingFigure(MyColor color, FigureType type);

    MyVector<Move> generateMoves(const Board& board, const Position& from) const override;

};

#pragma once
#include "Figure.h"

class SlidingFigure: public Figure
{
protected:
 
    MyVector<Move>generateSliding(const Board& board, const Position& from,
            const MyVector<MyPair<int, int>>& directions) const;

public:

    SlidingFigure(Color color, FigureType type);

    MyVector<Move> generateMoves(const Board& board, const Position& from) const override;

};

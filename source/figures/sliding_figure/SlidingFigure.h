#include "Figure.h"
#pragma once

class SlidingFigure: public Figure
{
protected:
 
    // generic N-direction ray-tracing helper (used by rook, bishop, queen)
    MyVector<Move>generateSliding(const Board& board, const Position& from, 
            const MyVector<MyPair<int, int>>& directions) const;

public:

    SlidingFigure(MyColor color, FigureType type);

    // base override does nothing-real pieces override again
    MyVector<Move> generateMoves(const Board& board, const Position& from) const override;

};

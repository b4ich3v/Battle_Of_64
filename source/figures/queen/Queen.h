#include "SlidingFigure.h"
#pragma once

class Queen: public SlidingFigure 
{
public:

    explicit Queen(MyColor color); // just forwards color

    MyVector<Move> generateMoves(const Board& board,const Position& from) const override; // QUEEN_DIRS

    void accept(Visitor& visitor) const override;

    char symbol() const override; // 'Q', 'q'

};


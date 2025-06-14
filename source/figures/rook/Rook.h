#include "SlidingFigure.h"
#pragma once

class Rook: public SlidingFigure 
{
public:

    explicit Rook(MyColor color);

    MyVector<Move> generateMoves(const Board& board, const Position& from) const override; // rook, horizontal and vertical rays

    void accept(Visitor& visitor) const override; // visitor double-dispatch

    char symbol() const override; // 'R', 'r'

};


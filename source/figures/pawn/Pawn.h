#include "Figure.h"
#pragma once

class Pawn: public Figure 
{
public:

    explicit Pawn(MyColor color);

    MyVector<Move> generateMoves(Board const& board, Position const& from) const override; // all pawn logic

    void accept(Visitor& visitor) const override;

    char symbol() const override; // 'P', 'p'

};



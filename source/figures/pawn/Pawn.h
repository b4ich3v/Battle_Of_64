#include "Figure.h"
#pragma once

class Pawn: public Figure 
{
public:

    explicit Pawn(Color color);

    MyVector<Move> generateMoves(Board const& board, Position const& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};



#include "Figure.h"
#pragma once

class Knight: public Figure 
{
public:

    explicit Knight(MyColor color); // ctor just forwards color

    MyVector<Move> generateMoves(const Board& board, const Position& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};


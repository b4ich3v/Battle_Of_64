#include "Figure.h"
#pragma once

class King: public Figure 
{
public:

    explicit King(MyColor color); // just sets color

    MyVector<Move> generateMoves(const Board& board, const Position& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};


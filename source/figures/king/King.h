#include "Figure.h"
#pragma once

class King: public Figure 
{
public:

    explicit King(Color color);

    MyVector<Position> generateMoves(const Board& board, const Position& from) const override;

    void accept(Visitor& visitor) const override;

    char symbol() const override;

};


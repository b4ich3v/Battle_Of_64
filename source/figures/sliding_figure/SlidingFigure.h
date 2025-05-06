#include "Figure.h"
#pragma once

class SlidingFigure: public Figure
{
protected:

    MyVector<Position> generateSliding(const Board& board, const Position& from,
        MyVector<std::pair<int, int>> directions) const;

public:

    SlidingFigure(Color color, FigureType type);

};


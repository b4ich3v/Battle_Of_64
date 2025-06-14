#include "Queen.h"

Queen::Queen(MyColor color): 
    SlidingFigure(color, FigureType::QUEEN) {}

MyVector<Move> Queen::generateMoves(const Board& board,
    const Position& from) const
{

    return generateSliding(board, from, QUEEN_DIRS); // 8 directions

}

void Queen::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Queen::symbol() const
{

    return (getColor() == MyColor::WHITE ? 'Q' : 'q');

}

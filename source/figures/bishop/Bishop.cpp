#include "Bishop.h"

Bishop::Bishop(MyColor color): 
    SlidingFigure(color, FigureType::BISHOP) {}

MyVector<Move> Bishop::generateMoves(const Board& board,
    const Position& from) const
{
    
    return generateSliding(board, from, DIAGONAL_DIRS); // reuse helper

}

void Bishop::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Bishop::symbol() const
{

    return (getColor() == MyColor::WHITE ? 'B' : 'b');

}

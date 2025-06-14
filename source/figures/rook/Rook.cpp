#include "Rook.h"

Rook::Rook(MyColor color): 
    SlidingFigure(color, FigureType::ROOK) {}

MyVector<Move> Rook::generateMoves(const Board& board,
    const Position& from) const
{

    return generateSliding(board, from, ROOK_DIRS); // 4 orthogonal rays

}

void Rook::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Rook::symbol() const
{

    return (getColor() == MyColor::WHITE ? 'R' : 'r'); // PGN letter

}

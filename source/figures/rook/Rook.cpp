#include "Rook.h"

static MyVector<MyPair<int, int>> makeRookDirs()
{

    MyVector<MyPair<int, int>> dirs;

    dirs.push_back(MyPair<int, int>(-1, 0));   
    dirs.push_back(MyPair<int, int>(1, 0));   
    dirs.push_back(MyPair<int, int>(0, -1));   
    dirs.push_back(MyPair<int, int>(0, 1));   

    return dirs;

}

static const MyVector<MyPair<int, int>> ROOK_DIRS = makeRookDirs();

Rook::Rook(Color color): 
    SlidingFigure(color, FigureType::ROOK) {}

MyVector<Move> Rook::generateMoves(const Board& board,
    const Position& from) const
{

    return generateSliding(board, from, ROOK_DIRS);

}

void Rook::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Rook::symbol() const
{

    return (getColor() == Color::WHITE ? 'R' : 'r');

}

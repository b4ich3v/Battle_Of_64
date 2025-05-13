#include "Bishop.h"

static MyVector<MyPair<int, int>> makeBishopDirs()
{

    MyVector<MyPair<int, int>> dirs;

    dirs.push_back(MyPair<int, int>(-1, -1));
    dirs.push_back(MyPair<int, int>(-1, 1));
    dirs.push_back(MyPair<int, int>(1, -1));
    dirs.push_back(MyPair<int, int>(1, 1));

    return dirs;

}

static const MyVector<MyPair<int, int>> BISHOP_DIRS = makeBishopDirs();

Bishop::Bishop(Color color): 
    SlidingFigure(color, FigureType::BISHOP) {}

MyVector<Move> Bishop::generateMoves(const Board& board,
    const Position& from) const
{
    
    return generateSliding(board, from, BISHOP_DIRS);

}

void Bishop::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Bishop::symbol() const
{

    return (getColor() == Color::WHITE ? 'B' : 'b');

}

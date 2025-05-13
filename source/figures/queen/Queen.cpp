#include "Queen.h"

static MyVector<MyPair<int, int>> makeQueenDirs()
{

    MyVector<MyPair<int, int>> dirs;
   
    dirs.push_back(MyPair<int, int>(-1, 0));
    dirs.push_back(MyPair<int, int>(1, 0));
    dirs.push_back(MyPair<int, int>(0, -1));
    dirs.push_back(MyPair<int, int>(0, 1));
    
    dirs.push_back(MyPair<int, int>(-1, -1));
    dirs.push_back(MyPair<int, int>(-1, 1));
    dirs.push_back(MyPair<int, int>(1, -1));
    dirs.push_back(MyPair<int, int>(1, 1));

    return dirs;

}

static const MyVector<MyPair<int, int>> QUEEN_DIRS = makeQueenDirs();

Queen::Queen(Color color): 
    SlidingFigure(color, FigureType::QUEEN) {}

MyVector<Move> Queen::generateMoves(const Board& board,
    const Position& from) const
{

    return generateSliding(board, from, QUEEN_DIRS);

}

void Queen::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Queen::symbol() const
{

    return (getColor() == Color::WHITE ? 'Q' : 'q');

}

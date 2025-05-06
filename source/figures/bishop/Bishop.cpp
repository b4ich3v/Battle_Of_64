#include "Bishop.h"

Bishop::Bishop(Color color): 
    SlidingFigure(color, FigureType::BISHOP) {}

MyVector<Position> Bishop::generateMoves(const Board& board,const Position& from) const
{

    static const MyVector<MyPair<int, int>> dirs = []
    {

        MyVector<MyPair<int, int>> directions;

        directions.push_back({ 1,  1 });
        directions.push_back({ 1, -1 });
        directions.push_back({ -1, 1 });
        directions.push_back({ -1, -1 });

        return directions;

    }();

    return generateSliding(board, from, dirs);

}

char Bishop::symbol() const 
{

    return (color == Color::WHITE ? 'B' : 'b');

}

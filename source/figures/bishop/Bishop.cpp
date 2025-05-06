#include "Bishop.h"

Bishop::Bishop(Color color):
    SlidingFigure(color, FigureType::BISHOP) {}

MyVector<Position> Bishop::generateMoves(const Board& board,const Position& from) const
{

    static const MyVector<std::pair<int, int>> dirs = []
    {

        MyVector<std::pair<int, int>> directions;

        directions.push_back(std::make_pair(1, 1));
        directions.push_back(std::make_pair(1, -1));
        directions.push_back(std::make_pair(-1, 1));
        directions.push_back(std::make_pair(-1, -1));

        return directions;

    }();

    return generateSliding(board, from, dirs);

}

char Bishop::symbol() const
{

    return (color == Color::WHITE ? 'B' : 'b');

}

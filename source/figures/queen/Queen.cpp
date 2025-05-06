#include "Queen.h"

Queen::Queen(Color color):
    SlidingFigure(color, FigureType::QUEEN) {}

MyVector<Position> Queen::generateMoves(const Board& board, const Position& from) const
{

    static const MyVector<std::pair<int, int>> dirs = []
    {

        MyVector<std::pair<int, int>> directions;
        std::pair<int, int> arr[] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };

        for (auto& p : arr) directions.push_back(p);

        return directions;

    }();

    return generateSliding(board, from, dirs);

}

char Queen::symbol() const
{

    return (color == Color::WHITE ? 'Q' : 'q');

}

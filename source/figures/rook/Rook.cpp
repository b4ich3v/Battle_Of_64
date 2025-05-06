#include "Rook.h"

Rook::Rook(Color color): 
    SlidingFigure(color, FigureType::ROOK) {}

MyVector<Position> Rook::generateMoves(const Board& board, const Position& from) const
{

    static const MyVector<MyPair<int, int>> dirs = [] 
    {

        MyVector<MyPair<int, int>> directions;

        directions.push_back({ 1, 0 });
        directions.push_back({ -1, 0 });
        directions.push_back({ 0, 1 });
        directions.push_back({ 0, -1 });

        return directions;

    }();

    return generateSliding(board, from, dirs);

}

char Rook::symbol() const 
{

    return (color == Color::WHITE ? 'R' : 'r');

}

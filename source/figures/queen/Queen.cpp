#include "Queen.h"

Queen::Queen(Color color): 
    SlidingFigure(color, FigureType::QUEEN) {}

MyVector<Position> Queen::generateMoves(const Board& board, const Position& from) const
{

    static const MyVector<MyPair<int, int>> dirs = [] 
    {

        MyVector<MyPair<int, int>> directions;
        MyPair<int, int> arr[] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };

        int dirCount = sizeof(arr) / sizeof(arr[0]);

        for (int i = 0; i < dirCount; i++) 
        {

            directions.push_back(arr[i]);

        }

        return directions;

    }();

    return generateSliding(board, from, dirs);

}

char Queen::symbol() const 
{

    return (color == Color::WHITE ? 'Q' : 'q');

}

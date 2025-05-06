#include "SlidingFigure.h"
#include "Board.h"

SlidingFigure::SlidingFigure(Color color, FigureType type)
    : Figure(color, type) {}

MyVector<Position> SlidingFigure::generateSliding(const Board& board, const Position& from,
    MyVector<MyPair<int, int>> directions) const
{

    MyVector<Position> moves;
    size_t directionCount = directions.size();

    for (size_t i = 0; i < directionCount; i++) 
    {

        int dx = directions[i].first;
        int dy = directions[i].second;

        for (int step = 1; step < 8; step++)
        {

            Position to;

            to.row = from.row + dx * step;
            to.col = from.col + dy * step;

            if (!board.isValid(to)) break;
            moves.push_back(to);
            if (board.at(to) != nullptr) break;

        }

    }

    return moves;

}

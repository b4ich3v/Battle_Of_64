#include "SlidingFigure.h"
#include "Board.h"

SlidingFigure::SlidingFigure(MyColor color, FigureType type): 
    Figure(color, type) {}

MyVector<Move> SlidingFigure::generateSliding(const Board& board, const Position& from,
    const MyVector<MyPair<int, int>>& directions) const
{

    MyVector<Move> result;

    for (size_t i = 0; i < directions.size(); i++) // each compass ray
    {

        int deltaRow = directions[i].first;
        int deltaCol = directions[i].second;

        Position currentPosition = from;

        while (true) 
        {

            currentPosition.row += (int8_t)(deltaRow);
            currentPosition.col += (int8_t)(deltaCol);

            if (!board.isValid(currentPosition)) break; // outside board

            const Figure* target = board.at(currentPosition);

            if (target == nullptr || target->getColor() != getColor()) // empty or capture
                result.push_back(Move(from, currentPosition));   

            if (target != nullptr) break; // stop at first piece

        }

    }

    return result;

}

// base class returns empty-concrete piece must call generateSliding()
MyVector<Move> SlidingFigure::generateMoves(const Board&,
    const Position&) const
{

    return {};   

}

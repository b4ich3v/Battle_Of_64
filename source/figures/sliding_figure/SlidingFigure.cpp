#include "SlidingFigure.h"
#include "Board.h"

SlidingFigure::SlidingFigure(MyColor color, FigureType type): 
    Figure(color, type) {}

MyVector<Move> SlidingFigure::generateSliding(const Board& board, const Position& from,
    const MyVector<MyPair<int, int>>& directions) const
{

    MyVector<Move> result;

    for (size_t i = 0; i < directions.size(); i++)
    {

        int deltaRow = directions[i].first;
        int deltaCol = directions[i].second;

        Position currentPosition = from;

        while (true) 
        {

            currentPosition.row += (int8_t)(deltaRow);
            currentPosition.col += (int8_t)(deltaCol);

            if (!board.isValid(currentPosition)) break;

            const Figure* target = board.at(currentPosition);

            if (target == nullptr || target->getColor() != getColor())
                result.push_back(Move(from, currentPosition));   

            if (target != nullptr) break;           
        }

    }

    return result;

}

MyVector<Move> SlidingFigure::generateMoves(const Board&,
    const Position&) const
{

    return {};   

}

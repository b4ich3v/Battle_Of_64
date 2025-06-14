#include "Knight.h"
#include "Board.h"

Knight::Knight(MyColor color): 
    Figure(color, FigureType::KNIGHT) {}

MyVector<Move> Knight::generateMoves(const Board& board,
    const Position& from) const
{

    MyVector<Move> move; // result container

    for (int i = 0; i < ROWS_COUNT; i++) // 8 L-jumps
    {

        int8_t deltaRow = KNIGHT_DIRS[i].first;
        int8_t deltoCol = KNIGHT_DIRS[i].second;

        Position to{ (int8_t)(from.row + deltaRow),
                     (int8_t)(from.col + deltoCol) };

        if (!board.isValid(to)) continue;

        const Figure* currentFigure = board.at(to);

        if (currentFigure == nullptr || currentFigure->getColor() != getColor()) // empty or enemy
            move.push_back(Move(from, to));

    }

    return move;

}

void Knight::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char Knight::symbol() const
{

    return (color == MyColor::WHITE ? 'N' : 'n'); // PGN letter

}

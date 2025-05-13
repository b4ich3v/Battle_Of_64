#include "Knight.h"
#include "Board.h"

Knight::Knight(Color color): 
    Figure(color, FigureType::KNIGHT) {}

MyVector<Move> Knight::generateMoves(const Board& board,
    const Position& from) const
{

    static const int8_t delta[8][2] = 
    {

        {-2,-1},{-2,1},{-1,-2},{-1,2},
        {1,-2},{1,2},{2,-1},{2,1}

    };

    MyVector<Move> move;

    for (int i = 0; i < 8; i++)
    {

        int8_t deltaRow = delta[i][0];
        int8_t deltoCol = delta[i][1];

        Position to{ (int8_t)(from.row + deltaRow),
                     (int8_t)(from.col + deltoCol) };

        if (!board.isValid(to)) continue;

        const Figure* currentFigure = board.at(to);

        if (currentFigure == nullptr || currentFigure->getColor() != getColor())
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

    return (color == Color::WHITE ? 'N' : 'n');

}

#include "King.h"
#include "Board.h"

King::King(Color color):
    Figure(color, FigureType::KING) {}

MyVector<Position> King::generateMoves(const Board& board, const Position& from) const
{

    static const std::pair<int, int> offsets[] =
    {

        {1, 0}, {1, 1}, {0, 1}, {-1, 1},
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}

    };

    MyVector<Position> moves;

    for (int i = 0; i < 8; i++)
    {

        Position to { from.row + offsets[i].first, from.col + offsets[i].second };

        if (!board.isValid(to)) continue;

        Figure* occupant = board.at(to);

        if (occupant == nullptr || occupant->getColor() != this->getColor()) moves.push_back(to);

    }

    if (board.canCastleKingSide(color))
    {

        Position ks{ from.row, from.col + 2 };
        if (board.isValid(ks)) moves.push_back(ks);

    }

    if (board.canCastleQueenSide(color))
    {

        Position qs{ from.row, from.col - 2 };
        if (board.isValid(qs)) moves.push_back(qs);

    }

    return moves;

}

char King::symbol() const
{

    return (color == Color::WHITE ? 'K' : 'k');

}

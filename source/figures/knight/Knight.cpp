#include "Knight.h"
#include "Board.h"

Knight::Knight(Color color): 
    Figure(color, FigureType::KNIGHT) {}

MyVector<Position> Knight::generateMoves(const Board& board, const Position& from) const
{

    static const MyPair<int, int> offsets[] = 
    {

        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}

    };

    MyVector<Position> moves;

    for (int i = 0; i < 8; i++)
    {

        Position to{ from.row + offsets[i].first, from.col + offsets[i].second };

        if (!board.isValid(to)) continue;

        Figure* occupant = board.at(to);
        
        if (occupant == nullptr || occupant->getColor() != this->getColor())  moves.push_back(to);

    }

    return moves;

}

char Knight::symbol() const
{

    return (color == Color::WHITE ? 'N' : 'n');

}

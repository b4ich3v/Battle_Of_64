#include "King.h"
#include "Board.h"

King::King(Color color): 
    Figure(color, FigureType::KING) {}

MyVector<Move> King::generateMoves(const Board& board,
    const Position& from) const
{

    static const MyPair<int, int> offsets[8] = 
    {

        { 1, 0}, { 1, 1}, { 0, 1}, {-1, 1},
        {-1, 0}, {-1,-1}, { 0,-1}, { 1,-1}

    };

    MyVector<Move> move;

    for (int i = 0; i < 8; i++)
    {

        Position to{ (int8_t)(from.row + offsets[i].first),
                     (int8_t)(from.col + offsets[i].second) };

        if (!board.isValid(to)) continue;

        const Figure* occupied = board.at(to);

        if (occupied == nullptr || occupied->getColor() != getColor())
            move.push_back(Move(from, to));          

    }

    if (board.canCastleKingSide(getColor())) 
    {

        Position kingSide{ from.row, from.col + 2 };    
        move.push_back(Move(from, kingSide, SpecialMove::CASTLING_KING_SIDE));

    }

    if (board.canCastleQueenSide(getColor())) 
    {

        Position queenSide{ from.row, from.col - 2 };     
        move.push_back(Move(from, queenSide, SpecialMove::CASTLING_QUEEN_SIDE));

    }

    return move;

}

void King::accept(Visitor& visitor) const
{

    visitor.visit(*this);

}

char King::symbol() const
{

    return (getColor() == Color::WHITE ? 'K' : 'k');

}

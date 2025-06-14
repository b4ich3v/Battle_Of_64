#include "King.h"
#include "Board.h"

King::King(MyColor color): 
    Figure(color, FigureType::KING) {}

MyVector<Move> King::generateMoves(const Board& board,
    const Position& from) const
{

    MyVector<Move> move;

    for (size_t i = 0; i < ROWS_COUNT; i++) // 8 king steps
    {

        Position to{ (int8_t)(from.row + KING_DIRS[i].first),
                     (int8_t)(from.col + KING_DIRS[i].second) };

        if (!board.isValid(to)) continue;

        const Figure* occupied = board.at(to);

        if (occupied == nullptr || occupied->getColor() != getColor())
            move.push_back(Move(from, to));          

    }

    if (board.canCastleKingSide(getColor())) // O-O
    {

        Position kingSide{ from.row, from.col + KING_CASTLE_SHIFT };
        move.push_back(Move(from, kingSide, SpecialMove::CASTLING_KING_SIDE));

    }

    if (board.canCastleQueenSide(getColor())) // O-O-O
    {

        Position queenSide{ from.row, from.col - KING_CASTLE_SHIFT };
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

    return (getColor() == MyColor::WHITE ? 'K' : 'k'); // PGN letter

}

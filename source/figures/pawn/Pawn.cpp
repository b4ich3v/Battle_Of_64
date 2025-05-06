#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(Color color):
    Figure(color, FigureType::PAWN) {}

MyVector<Position> Pawn::generateMoves(Board const& board, Position const& from) const
{

    MyVector<Position> moves;
    int dir = (color == Color::WHITE ? -1 : +1);
    Position one{ from.row + dir, from.col };

    if (board.isValid(one) && board.at(one) == nullptr)
    {

        if ((color == Color::WHITE && one.row == 0) ||
            (color == Color::BLACK && one.row == 7))
        {

            moves.push_back(Move(from, one, SpecialMove::PROMOTION, FigureType::QUEEN).to);

        }
        else
        {

            moves.push_back(one);

        }

        bool home = (color == Color::WHITE && from.row == 6) || (color == Color::BLACK && from.row == 1);
        Position two{ from.row + 2 * dir, from.col };

        if (home && board.isValid(two) && board.at(two) == nullptr)
        {

            moves.push_back(two);

        }

    }

    for (int dc = -1; dc <= 1; dc += 2)
    {

        Position cap{ from.row + dir, from.col + dc };

        if (!board.isValid(cap)) continue;

        Figure* occ = board.at(cap);

        if (occ && occ->getColor() != color)
        {

            if ((color == Color::WHITE && cap.row == 0) ||
                (color == Color::BLACK && cap.row == 7))
            {

                moves.push_back(cap);

            }
            else
            {

                moves.push_back(cap);

            }

        }

        Position behind{ from.row, from.col + dc };
        Figure* adj = board.isValid(behind) ? board.at(behind) : nullptr;

        if (adj && adj->getType() == FigureType::PAWN && adj->getColor() != color)
        {

            auto last = board.peekHistory();

            if (last.move.special == SpecialMove::NORMAL &&
                last.move.to == behind &&
                abs(last.move.from.row - last.move.to.row) == 2)
            {

                moves.push_back(cap);

            }

        }

    }

    return moves;

}

char Pawn::symbol() const
{

    return (color == Color::WHITE ? 'P' : 'p');

}

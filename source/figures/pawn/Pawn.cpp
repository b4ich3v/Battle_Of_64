#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(Color color): 
    Figure(color, FigureType::PAWN) {}

MyVector<Move> Pawn::generateMoves(const Board& board,
    const Position& from) const
{

    MyVector<Move> move;
    int8_t dir = (getColor() == Color::WHITE ? -1 : 1);
    Position one{ (int8_t)(from.row + dir), from.col };

    if (board.isValid(one) && board.at(one) == nullptr) 
    {

        bool promo = (one.row == 0 || one.row == 7);

        move.push_back(Move(from, one, promo ? SpecialMove::PROMOTION:
            SpecialMove::NORMAL, promo ? FigureType::QUEEN: FigureType::NONE));

        bool onHome = (getColor() == Color::WHITE && from.row == 6) ||
            (getColor() == Color::BLACK && from.row == 1);

        if (onHome) 
        {

            Position two{ static_cast<int8_t>(from.row + 2 * dir), from.col };

            if (board.at(two) == nullptr)
                move.push_back(Move(from, two, SpecialMove::DOUBLE_PAWN));

        }

    }

    for (int deltaCol = -1; deltaCol <= 1; deltaCol += 2)
    {

        Position capPosition{ (int8_t)(from.row + dir),
            (int8_t)(from.col + deltaCol) };

        if (!board.isValid(capPosition)) continue;

        const Figure* target = board.at(capPosition);
        bool promo = (capPosition.row == 0 || capPosition.row == 7);

        if (target && target->getColor() != getColor())
        {

            move.push_back(Move(from, capPosition, promo ? SpecialMove::PROMOTION :
                SpecialMove::NORMAL, promo ? FigureType::QUEEN : FigureType::NONE));

        }

        if (board.isEnPassantSquare(capPosition, getColor()))
        {

            move.push_back(Move(from, capPosition, SpecialMove::EN_PASSANT));

        }

    }

    return move;

}

void Pawn::accept(Visitor& visitor) const 
{

    visitor.visit(*this); 

}

char Pawn::symbol() const
{

    return (getColor() == Color::WHITE ? 'P' : 'p');

}

#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(MyColor color): 
    Figure(color, FigureType::PAWN) {}

MyVector<Move> Pawn::generateMoves(const Board& board,
    const Position& from) const
{

    MyVector<Move> move;
    int8_t dir = (getColor() == MyColor::WHITE ? EP_DIR_WHITE : EP_DIR_BLACK); // forward step
    Position one{ (int8_t)(from.row + dir), from.col }; // 1-square push

    if (board.isValid(one) && board.at(one) == nullptr) 
    {

        bool promo = (one.row == WHITE_PROMOTE_ROW || one.row == BLACK_PROMOTE_ROW); // promotion

        move.push_back(Move(from, one, promo ? SpecialMove::PROMOTION: 
            SpecialMove::NORMAL, promo ? FigureType::QUEEN: FigureType::NONE));

        bool onHome = (getColor() == MyColor::WHITE && from.row == WHITE_PAWN_ROW) || // start rank
            (getColor() == MyColor::BLACK && from.row == BLACK_PAWN_ROW);

        if (onHome) 
        {

            Position two{ (int8_t)(from.row + 2 * dir), from.col };

            if (board.at(two) == nullptr) 
                move.push_back(Move(from, two, SpecialMove::DOUBLE_PAWN)); // 2-square push

        }

    }

    for (int deltaCol = -1; deltaCol <= 1; deltaCol += 2) // diagonals
    {

        Position capPosition{ (int8_t)(from.row + dir),
            (int8_t)(from.col + deltaCol) };

        if (!board.isValid(capPosition)) continue;

        const Figure* target = board.at(capPosition);
        bool promo = (capPosition.row == WHITE_PROMOTE_ROW || capPosition.row == BLACK_PROMOTE_ROW);

        if (target && target->getColor() != getColor()) // normal capture
        {

            move.push_back(Move(from, capPosition, promo ? SpecialMove::PROMOTION : 
                SpecialMove::NORMAL, promo ? FigureType::QUEEN : FigureType::NONE));

        }

        if (board.isEnPassantSquare(capPosition, getColor())) // en passant
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

    return (getColor() == MyColor::WHITE ? 'P' : 'p');

}

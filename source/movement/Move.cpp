#include "Move.h"
#include "Board.h"
#include "Figure.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"

Move::Move(): from{ 0,0 }, to{ 0,0 },
special(SpecialMove::NORMAL), promotionType(FigureType::QUEEN) {}

Move::Move(Position from, Position to,
    SpecialMove specialMove, FigureType type):
    from(from), to(to), special(specialMove), promotionType(type) {}

SpecialMove Move::getSpecial() const
{

    return special;

}

void Move::execute(Board& board) const
{

    Figure* movingFigure = board.at(from);
    Figure* captured = nullptr;

    switch (special)
    {

    case SpecialMove::NORMAL: // plain move, capture
    {

        captured = board.at(to);
        board.set(to, movingFigure);
        board.set(from, nullptr);

        break;

    }
    case SpecialMove::CASTLING_KING_SIDE: // king two steps right and rook hop
    {

        int row = from.row;

        board.set(to, movingFigure);
        board.set(from, nullptr);

        Position rookFrom{ row, KS_ROOK_FROM_COL }, rookTo{ row, KS_ROOK_TO_COL };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);

        break;

    }
    case SpecialMove::CASTLING_QUEEN_SIDE: // king two steps left and rook hop
    {

        int row = from.row;
        board.set(to, movingFigure);
        board.set(from, nullptr);

        Position rookFrom{ row, QS_ROOK_FROM_COL }, rookTo{ row, QS_ROOK_TO_COL };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);

        break;

    }
    case SpecialMove::EN_PASSANT: // capture pawn behind target square
    {

        board.set(to, movingFigure);
        board.set(from, nullptr);

        int8_t delta = (int8_t)(from.row - to.row);
        Position capPos{ (int8_t)(to.row + delta), to.col };

        captured = board.at(capPos);
        board.set(capPos, nullptr);

        break;

    }
    case SpecialMove::PROMOTION: // replace pawn with new piece
    {

        board.set(from, nullptr);
        captured = board.at(to);

        Figure* promo = nullptr;

        switch (promotionType)
        {

        case FigureType::QUEEN: promo = new Queen(movingFigure->getColor()); break;
        case FigureType::ROOK: promo = new Rook(movingFigure->getColor()); break;
        case FigureType::BISHOP: promo = new Bishop(movingFigure->getColor()); break;
        case FigureType::KNIGHT: promo = new Knight(movingFigure->getColor()); break;
        default: promo = new Queen(movingFigure->getColor()); break;

        }

        board.set(to, promo);

        break;

    }
    case SpecialMove::DOUBLE_PAWN: // two-square pawn push
    {

        captured = nullptr;
        board.set(to, movingFigure);
        board.set(from, nullptr);

        break;

    }
    default: break;

    }

    board.pushHistory({ *this, captured }); // record for undo()

}

void Move::undo(Board& board) const
{

    auto entry = board.popHistory(); // fetch last record
    const Move& move = entry.move;
    Figure* movingFigure;

    switch (move.special)
    {

    case SpecialMove::NORMAL: // restore piece, capture
    {

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, entry.captured);
        entry.captured = nullptr;

        break;

    }
    case SpecialMove::CASTLING_KING_SIDE: // un-castle
    {

        int row = move.from.row;

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);

        Position rookFrom{ row, KS_ROOK_TO_COL }, rookTo{ row, KS_ROOK_FROM_COL };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);
        entry.captured = nullptr;

        break;

    }
    case SpecialMove::CASTLING_QUEEN_SIDE: 
    {

        int row = move.from.row;
        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);

        Position rookFrom{ row, QS_ROOK_TO_COL }, rookTo{ row, QS_ROOK_FROM_COL };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);
        entry.captured = nullptr;

        break;

    }
    case SpecialMove::EN_PASSANT: // restore captured pawn
    {

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);

        Position capPosition{ move.from.row, move.to.col };
        board.set(capPosition, entry.captured);
        entry.captured = nullptr;

        break;

    }
    case SpecialMove::PROMOTION: // delete promoted, respawn pawn
    {

        Figure* promoted = board.at(move.to);
        MyColor color = promoted->getColor();
        delete promoted;

        Figure* pawn = new Pawn(color);
        board.set(move.from, pawn);
        board.set(move.to, entry.captured);
        entry.captured = nullptr;

        break;

    }
    case SpecialMove::DOUBLE_PAWN: // rewind two-step push
    {

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);
        entry.captured = nullptr;

        break;

    }
    default: break;

    }

}

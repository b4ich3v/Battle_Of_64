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

Move::Move(Position f, Position t,
    SpecialMove s,FigureType p):
    from(f), to(t), special(s), promotionType(p) {}

void Move::execute(Board& board) const
{

    Figure* movingFigure = board.at(from);
    Figure* captured = nullptr;

    switch (special)
    {

    case SpecialMove::NORMAL:
    {

        captured = board.at(to);
        board.set(to, movingFigure);
        board.set(from, nullptr);

        break;

    }
    case SpecialMove::CASTLING_KING_SIDE:
    {

        int row = from.row;

        board.set(to, movingFigure);
        board.set(from, nullptr);

        Position rookFrom{ row,7 }, rookTo{ row,5 };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);

        break;

    }
    case SpecialMove::CASTLING_QUEEN_SIDE:
    {

        int row = from.row;
        board.set(to, movingFigure);
        board.set(from, nullptr);

        Position rookFrom{ row,0 }, rookTo{ row,3 };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);

        break;

    }
    case SpecialMove::EN_PASSANT:
    {

        board.set(to, movingFigure);

        board.set(from, nullptr);
        int dir = (movingFigure->getColor() == Color::WHITE ? +1 : -1);
        Position capPos{ to.row + dir, to.col };
        captured = board.at(capPos);
        board.set(capPos, nullptr);

        break;

    }
    case SpecialMove::PROMOTION:
    {

        board.set(from, nullptr);
        captured = board.at(to);

        Figure* promo = nullptr;

        switch (promotionType)
        {

        case FigureType::QUEEN:  promo = new Queen(movingFigure->getColor()); break;
        case FigureType::ROOK:   promo = new Rook(movingFigure->getColor()); break;
        case FigureType::BISHOP: promo = new Bishop(movingFigure->getColor()); break;
        case FigureType::KNIGHT: promo = new Knight(movingFigure->getColor()); break;
        default: promo = new Queen(movingFigure->getColor()); break;
            
        }

        board.set(to, promo);

        break;

    }
    default: break;

    }

    board.pushHistory({ *this, captured });

}

void Move::undo(Board& board) const
{

    auto entry = board.popHistory();
    const Move& move = entry.move;
    Figure* movingFigure;

    switch (move.special)
    {

    case SpecialMove::NORMAL:
    {

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, entry.captured);

        break;

    }
    case SpecialMove::CASTLING_KING_SIDE:
    {

        int row = move.from.row;

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);

        Position rookFrom{ row,5 }, rookTo{ row,7 };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);

        break;

    }
    case SpecialMove::CASTLING_QUEEN_SIDE:
    {

        int row = move.from.row;
        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);

        Position rookFrom{ row,3 }, rookTo{ row,0 };
        Figure* rook = board.at(rookFrom);
        board.set(rookTo, rook);
        board.set(rookFrom, nullptr);

        break;

    }
    case SpecialMove::EN_PASSANT:
    {

        movingFigure = board.at(move.to);
        board.set(move.from, movingFigure);
        board.set(move.to, nullptr);

        int dir = (movingFigure->getColor() == Color::WHITE ? +1 : -1);
        Position capPos{ move.to.row + dir, move.to.col };
        board.set(capPos, entry.captured);

        break;

    }
    case SpecialMove::PROMOTION:
    {

        Figure* promoted = board.at(move.to);
        delete promoted;

        Figure* pawn = new Pawn(entry.captured ? entry.captured->getColor() : Color::WHITE);
        board.set(move.from, pawn);
        board.set(move.to, entry.captured);

        break;

    }
    default: break;

    }

}

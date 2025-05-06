#include "Move.h"
#include "Board.h"

Move::Move(): from{ 0,0 }, to{ 0,0 },
    special(SpecialMove::NORMAL), promotionType(FigureType::QUEEN) {}

Move::Move(Position f, Position t,
    SpecialMove s,FigureType p):
    from(f), to(t), special(s), promotionType(p) {}

void Move::execute(Board& board)
{

    Figure* figure = board.at(from);
    Figure* captured = board.at(to);

    board.set(to, figure);
    board.set(from, nullptr);
    board.pushHistory({ *this, captured });

}

void Move::undo(Board& board)
{

    auto history = board.popHistory();
    board.set(history.move.to, history.captured);
    board.set(history.move.from, board.at(history.move.to));

}

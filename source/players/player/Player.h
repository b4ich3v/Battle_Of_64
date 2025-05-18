#include "Board.h"
#include "Move.h"
#pragma once

class Player
{
protected:

    Move pending;

public:

    virtual Move getMove(Board& board, MyColor side) = 0;

    virtual void setPendingMove(const Move& move);

    virtual ~Player() = default;

};

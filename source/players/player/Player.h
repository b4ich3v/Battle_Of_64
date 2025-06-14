#include "Board.h"
#include "Move.h"
#pragma once

class Player
{
protected:

    Move pending; // last GUI move waiting to be played

public:

    virtual Move getMove(Board& board, MyColor side) = 0; // produce a legal move-to be overridden

    virtual void setPendingMove(const Move& move); // remember a move coming from the UI, network

    virtual ~Player() = default; // ensure proper polymorphic cleanup

};

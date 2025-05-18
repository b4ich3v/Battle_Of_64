#include "Player.h"
#pragma once

class HumanPlayer: public Player
{
public:

    Move getMove(Board& board, MyColor color) override;

    void setPendingMove(const Move& move) override;

};

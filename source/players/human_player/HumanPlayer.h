#include "Player.h"
#pragma once

class HumanPlayer: public Player
{
public:

    Move getMove(Board& board, MyColor color) override; // deliver the move that the GUI, user has just entered

    void setPendingMove(const Move& move) override; // called by the UI to pass the user’s move in

};

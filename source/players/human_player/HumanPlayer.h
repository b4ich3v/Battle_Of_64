#include "Player.h"
#include "Move.h"
#pragma once

class HumanPlayer: public Player
{
public:

    explicit HumanPlayer(const MyString& name = "Human");

    Move requestMove(Board& board, Color side) override;

};

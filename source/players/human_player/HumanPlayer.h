#pragma once
#include "Player.h"

class HumanPlayer: public Player 
{
public:

    explicit HumanPlayer(const MyString& name);

    static bool tryParseSquare(const char* buffer, Position& position);

    Move requestMove(const Board& board, Color mover) override;

};

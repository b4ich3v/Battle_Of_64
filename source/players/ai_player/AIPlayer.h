#include "Player.h"
#pragma once

class AIPlayer: public Player
{
private:

    int maxDepth = 0;

    int search(Board& board, int depth, int alpha, int beta, MyColor side);

    int evaluate(const Board& board, MyColor side) const;

    static int value(FigureType type);

public:

    explicit AIPlayer(int depth = 3);

    Move getMove(Board& board, MyColor side) override;

};


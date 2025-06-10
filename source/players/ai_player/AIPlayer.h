#include "Player.h"
#pragma once

class AIPlayer: public Player
{
private:

    int maxDepth = 0;
    static Move lastWhite;
    static Move lastBlack;

    int search(Board& board, int depth, int alpha, int beta, MyColor side);

    int evaluate(const Board& board, MyColor side) const;

    static int value(FigureType type);

public:

    explicit AIPlayer(int depth = DEFAULT_DEPTH);

    Move getMove(Board& board, MyColor side) override;

};


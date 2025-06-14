#include "Player.h"
#include <cstdlib>   
#pragma once

class AIPlayer: public Player
{
private:

    int maxDepth = 0; // search depth (plys)
    static Move lastWhite, lastBlack; // last moves to avoid repeats

    int noiseRange = 0;  
    int marginCp = 0;

    int search(Board& board, int depth, int alpha, int beta, MyColor side); // alpha-beta

    int evaluate(const Board& board, MyColor side) const; // material+noise

    static int value(FigureType type); // piece value

public:

    explicit AIPlayer(Difficulty difficulty = Difficulty::Medium); 

    Move getMove(Board& board, MyColor side) override;

};


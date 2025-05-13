#include "Player.h"
#include "MyPriorityQueue.hpp"
#include "MyVector.hpp"
#pragma once

struct Node 
{
public:

    int x = 0;
    int y = 0;
    int effort = 0;

    Node(int x, int y, int effort);

    bool operator < (const Node& other) const;

};

class AIPlayer: public Player 
{
public:

    explicit AIPlayer(const MyString& name = "AI");

    Move requestMove(Board& board, Color mover) override;

};

#include "Player.h"
#include "MyPriorityQueue.hpp"   
#include "MyVector.hpp"
#pragma once

struct Node 
{
public:

    int x = 0;
    int y = 0;
    int effort;
    
    Node(int x, int y, int effort);

    bool operator < (const Node& other) const;

};

int minimumEffortPath(MyVector<MyVector<int>>& heights);

class AIPlayer: public Player 
{
public:

    explicit AIPlayer(const MyString& name);

    Move requestMove(Board& board, Color mover) override;

};

#include "Move.h"
#include "Board.h"
#include "MyString.h"  
#pragma once

class Player
{
protected:

    MyString name;

public:
    
    explicit Player(const MyString& name);

    virtual ~Player() = default;

    const MyString& getName() const;

    virtual Move requestMove(Board& board, Color mover) = 0;

};

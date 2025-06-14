#include <iostream>
#pragma once

struct Position 
{
public:

    int8_t row = 0; // 0-7, rank from White’s side
    int8_t col = 0; // 0-7, file a-h

    Position(int8_t row = 0, int8_t col = 0);

    bool operator == (Position const& other) const; // exact same square

};



#include <iostream>
#pragma once

struct Position 
{
public:

    int8_t row = 0; 
    int8_t col = 0; 

    Position(int8_t row = 0, int8_t col = 0);

    bool operator == (Position const& other) const;

};

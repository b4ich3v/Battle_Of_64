#include <iostream>
#pragma once

struct Position
{
public:

    uint8_t row = 0;
    uint8_t col = 0;

    Position(uint8_t row = 0, uint8_t col = 0);

    bool operator == (Position const& other) const;

};



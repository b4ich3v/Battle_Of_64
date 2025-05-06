#include "Position.h"

Position::Position(int8_t row, int8_t col):
    row(row), col(col) {}

bool Position::operator == (Position const& other) const 
{

    return row == other.row && col == other.col;

}

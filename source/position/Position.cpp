#include "Position.h"

Position::Position(uint8_t row, uint8_t col):
    row(row), col(col) {}

bool Position::operator == (Position const& other) const
{

    return row == other.row && col == other.col;

}

#include "Position.h"
#include "../MyVector.hpp"
#pragma once

enum class Color: uint8_t
{

    WHITE,
    BLACK

};

enum class FigureType: uint8_t
{

    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING

};

class Board;

class Figure
{
protected:

    Color color;
    FigureType type;

public:

    Figure(Color color, FigureType type);

    Color getColor() const;

    FigureType getType() const;

    virtual MyVector<Position> generateMoves(const Board& board, const Position& from) const = 0;

    virtual char symbol() const = 0;

    virtual ~Figure() = default;

};

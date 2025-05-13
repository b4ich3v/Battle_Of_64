#include "Position.h"
#include "Visitor.h"
#include "MyVector.hpp"
#include "MyPair.hpp"
#pragma once

class Move;

enum class Color: uint8_t
{

    WHITE,
    BLACK

};

inline Color oppositeColor(Color color)
{

    return color == Color::WHITE ? Color::BLACK : Color::WHITE;

}

enum class FigureType: uint8_t
{

    NONE,
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
    Position position;

public:

    Figure(Color color, FigureType type);

    Color getColor() const;

    FigureType getType() const;

    Position getPosition() const;

    void setPosition(const Position& position);

    virtual MyVector<Move> generateMoves(const Board& board, const Position& from) const = 0;

    virtual char symbol() const = 0;

    virtual void accept(Visitor& visitor) const = 0;

    virtual ~Figure() = default;

};

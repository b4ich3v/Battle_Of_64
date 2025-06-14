#include "Position.h"
#include "Visitor.h"
#include "MyVector.hpp"
#include "MyPair.hpp"
#include "Types.h"
#pragma once

class Board;

class Move;

inline MyColor oppositeColor(MyColor color)
{

    return color == MyColor::WHITE ? MyColor::BLACK : MyColor::WHITE;

}

class Figure
{
protected:

    MyColor color; // piece side: WHITE or BLACK
    FigureType type; // enum value KING, KNIGHT and so on
    Position position; // current square

public:

    Figure(MyColor color, FigureType type);

    MyColor getColor() const;

    FigureType getType() const;

    Position getPosition() const;

    void setPosition(const Position& position);

    virtual MyVector<Move> generateMoves(const Board& board, const Position& from) const = 0; // piece logic

    virtual char symbol() const = 0; // PGN char

    virtual void accept(Visitor& visitor) const = 0; // for draw, UI

    virtual ~Figure() = default;

};

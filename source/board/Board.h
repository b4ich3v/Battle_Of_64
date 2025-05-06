#include "Position.h"
#include "Figure.h"
#include "Move.h"
#pragma once

struct HistoryEntry
{
public:

    Move move;
    Figure* captured;

    HistoryEntry();

    HistoryEntry(const Move& move, Figure* captured);

};

class Board
{
private:

    MyVector<MyVector<Figure*>> table;
    MyVector<HistoryEntry> history;

public:

    Board();

    bool isValid(const Position& position) const;

    Figure* at(const Position& position) const;

    void set(const Position& position, Figure* figure);

    void applyMove(const Move& move);

    void undoMove(const Move& move);

    void pushHistory(HistoryEntry entry);

    bool canCastleKingSide(Color color) const;

    bool canCastleQueenSide(Color color) const;

    HistoryEntry peekHistory() const;

    HistoryEntry popHistory();

};


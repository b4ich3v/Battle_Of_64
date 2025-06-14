#include "Figure.h"
#include "Move.h"
#pragma once

struct HistoryEntry
{
public:

    Move move; // move just played
    Figure* captured; // piece captured (may be null)

    // castling rights "before" the move-for undo
    MyVector<bool> castleKingSide;
    MyVector<bool> castleQueenSide;

    HistoryEntry(); // empty

    HistoryEntry(const Move& move, Figure* captured); // no castles

    HistoryEntry(const Move& move, Figure* captured, // full info
        const MyVector<bool>& prevKing, const MyVector<bool>& prevQueen);

};


#include "Figure.h"
#include "Move.h"
#pragma once

struct HistoryEntry
{
public:

    Move move;
    Figure* captured;

    MyVector<bool> castleKingSide;
    MyVector<bool> castleQueenSide;

    HistoryEntry();

    HistoryEntry(const Move& move, Figure* captured);

    HistoryEntry(const Move& move, Figure* captured,
        const MyVector<bool>& prevKing, const MyVector<bool>& prevQueen);

};


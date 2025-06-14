#include "HistoryEntry.h"

HistoryEntry::HistoryEntry(): // dummy entry
    move(Position{ 0,0 }, Position{ 0,0 }), captured(nullptr) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured): // no castles
    move(move), captured(captured) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured,
    const MyVector<bool>& prevKing, const MyVector<bool>& prevQueen): // full undo data
    move(move), captured(captured),
    castleKingSide(prevKing), castleQueenSide(prevQueen) {}

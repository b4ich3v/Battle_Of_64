#include "HistoryEntry.h"

HistoryEntry::HistoryEntry():
    move(Position{ 0,0 }, Position{ 0,0 }), captured(nullptr) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured):
    move(move), captured(captured) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured,
    const MyVector<bool>& prevKing, const MyVector<bool>& prevQueen) :
    move(move), captured(captured),
    castleKingSide(prevKing), castleQueenSide(prevQueen) {}

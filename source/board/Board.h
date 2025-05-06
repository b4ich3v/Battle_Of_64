#include "Position.h"
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
        const MyVector<bool>&prevKing, const MyVector<bool>&prevQueen);

};

class Board 
{
private:

    MyVector<bool> castleKS;
    MyVector<bool> castleQS;

    MyVector<MyVector<Figure*>> table; 
    MyVector<HistoryEntry> history;

public:

    Board();

    bool isValid(const Position& position) const;

    Figure* at(const Position& position) const;

    void set(const Position& position, Figure* figure);

    void setupInitialPosition();

    void applyMove(const Move& move);

    void undoMove(const Move& move);

    void pushHistory(HistoryEntry entry);

    bool canCastleKingSide(Color color) const;

    bool canCastleQueenSide(Color color) const;

    bool isUnderAttack(const Position& position, Color attacker) const;

    bool isInCheck(Color color) const;

    bool isLegalMove(const Move& move, Color moverSide);

    HistoryEntry peekHistory() const;

    HistoryEntry popHistory();

};


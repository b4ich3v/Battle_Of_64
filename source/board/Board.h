#include "Position.h"
#include "Writer.h"
#include "HistoryEntry.h"
#include "Reader.h"
#include "VisitorVisualization.h"
#pragma once

class Board
{
private:

    MyVector<bool> castleKS;
    MyVector<bool> castleQS;

    MyVector<MyVector<Figure*>> table;
    MyVector<HistoryEntry> history;

    Board();

    ~Board();

    Board(const Board&) = delete;

    Board& operator = (const Board&) = delete;

    void free();

public:

    static Board& instance();

    bool isValid(const Position& position) const;

    Figure* at(const Position& position) const;

    void set(const Position& position, Figure* figure);

    void accept(Visitor& visitor) const;

    void setupInitialPosition();

    void clear();

    void applyMove(const Move& move);

    void undoMove(const Move& move);

    void pushHistory(HistoryEntry entry);

    bool canCastleKingSide(MyColor color) const;

    bool canCastleQueenSide(MyColor color) const;

    bool isUnderAttack(const Position& position, MyColor attacker) const;

    bool isInCheck(MyColor color) const;

    bool hasLegalMoves(MyColor side);          

    bool isEnPassantSquare(const Position& cap, MyColor pawnColor) const;

    MyVector<Move> generateAllLegalMoves(MyColor side);

    bool isLegalMove(const Move& move, MyColor side);

    HistoryEntry peekHistory() const;

    HistoryEntry popHistory();

    void serialize(Writer& writer) const;

    void deserialize(Reader& reader);

    static Piece figureToPiece(const Figure* figure);

    static Figure* createFigureFromPiece(Piece piece);

};

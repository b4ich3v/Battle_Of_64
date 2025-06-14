#include "Position.h"
#include "Writer.h"
#include "HistoryEntry.h"
#include "Reader.h"
#include "VisitorVisualization.h"
#pragma once

class Board
{
private:

    MyVector<bool> castleKS; // castling rights (king-side)
    MyVector<bool> castleQS; // castling rights (queen-side)

    MyVector<MyVector<Figure*>> table; // 8×8 piece pointers
    MyVector<HistoryEntry> history; // move stack for undo

    Board(); // singleton-use instance()

    Board(const Board&) = delete; // no copies

    Board& operator = (const Board&) = delete;

    ~Board();

    void free(); // delete pieces + history

public:

    static Board& instance(); // global board object

    void setupInitialPosition();

    bool isValid(const Position& position) const; // is inside in the board check

    Figure* at(const Position& position) const; // read square

    void set(const Position& position, Figure* figure); // write square

    void accept(Visitor& visitor) const; 

    void clear();

    void applyMove(const Move& move);

    void undoMove(const Move& move);

    void pushHistory(HistoryEntry entry);

    bool canCastleKingSide(MyColor color) const;

    bool canCastleQueenSide(MyColor color) const;

    bool isUnderAttack(const Position& position, MyColor attacker) const; // is square attacked

    bool isInCheck(MyColor color) const; // is king in check

    bool hasLegalMoves(MyColor side); // any moves left    

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

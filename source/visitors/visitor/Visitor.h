#pragma once

class Board;
class Rook;
class Knight;
class Bishop;
class Queen;
class King;
class Pawn;

class Visitor 
{
public:

    virtual void visit(const Board& board) = 0; // top–level scene (full board)

    virtual void visit(const Rook& rook) = 0;

    virtual void visit(const Knight& knight) = 0;

    virtual void visit(const Bishop& bishop) = 0;

    virtual void visit(const Queen& queen) = 0;

    virtual void visit(const King& king) = 0;

    virtual void visit(const Pawn& pawn) = 0;

    virtual ~Visitor() = default; // polymorphic delete

};

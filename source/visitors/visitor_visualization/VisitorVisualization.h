#include <windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include "Visitor.h"
#include "Figure.h"
#include "Position.h"
#pragma once

enum class Piece: uint8_t
{

    NONE,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK

};

class VisitorVisualization: public Visitor
{
private:


    std::unordered_map<Piece, Gdiplus::Image*> images;
    Gdiplus::Graphics* graphics;
    int size = 0;

    void loadImages();

    void freeImages();

    void drawBoard();

    void drawFigure(const Figure& figure);

public:

    explicit VisitorVisualization(int squareSize = 80);

    ~VisitorVisualization();

    void setGraphics(Gdiplus::Graphics* graphics);

    Gdiplus::Image* getImage(Piece piece) const;

    Piece figureToPiece(const Figure& figure) const;

    void visit(const Board& board) override;

    void visit(const Rook& rook) override;

    void visit(const Knight& knight) override;

    void visit(const Bishop& bishop) override;

    void visit(const Queen& queen) override;

    void visit(const King& king) override;

    void visit(const Pawn& pawn) override;

};


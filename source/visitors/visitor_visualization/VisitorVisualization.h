#include <windows.h>
#include <gdiplus.h>
#include <unordered_map>
#include "Visitor.h"
#include "Figure.h"
#include "Position.h"
#pragma once

class VisitorVisualization: public Visitor 
{
private:


    std::unordered_map<Piece, Gdiplus::Image*> images; // sprite cache
    Gdiplus::Graphics* graphics; // current target
    int size = 0; // square-pixel side

    void loadImages(); // PNG to Image* once

    void freeImages(); // delete all on shutdown

    void drawBoard(); // draw 8×8 background

    void drawFigure(const Figure& figure); // blit one sprite

public:

    explicit VisitorVisualization(int squareSize = 80); // px per square

    ~VisitorVisualization();

    void setGraphics(Gdiplus::Graphics* graphics); // choose target surface

    Gdiplus::Image* getImage(Piece piece) const; // lookup sprite

    Piece figureToPiece(const Figure& figure) const; // Figure to enum helper

    void visit(const Board& board) override;

    void visit(const Rook& rook) override;

    void visit(const Knight& knight) override;

    void visit(const Bishop& bishop) override;

    void visit(const Queen& queen) override;

    void visit(const King& king) override;

    void visit(const Pawn& pawn) override;

};

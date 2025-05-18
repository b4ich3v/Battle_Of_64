#include "VisitorVisualization.h"
#include "Board.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

using namespace Gdiplus;

VisitorVisualization::VisitorVisualization(int squareSize):
    size(squareSize), graphics(nullptr)
{

    loadImages();

}

VisitorVisualization::~VisitorVisualization()
{

    freeImages();

}

void VisitorVisualization::setGraphics(Graphics* newG)
{

    graphics = newG;

}

Image* VisitorVisualization::getImage(Piece piece) const
{

    auto iter = images.find(piece);
    return iter == images.end() ? nullptr : iter->second;

}

void VisitorVisualization::visit(const Board& board) { drawBoard(); }

void VisitorVisualization::visit(const Rook& rook) { drawFigure(rook); }

void VisitorVisualization::visit(const Knight& knight) { drawFigure(knight); }

void VisitorVisualization::visit(const Bishop& bishop) { drawFigure(bishop); }

void VisitorVisualization::visit(const Queen& queen) { drawFigure(queen); }

void VisitorVisualization::visit(const King& king) { drawFigure(king); }

void VisitorVisualization::visit(const Pawn& pawn) { drawFigure(pawn); }

void VisitorVisualization::loadImages()
{

    images[Piece::WP] = Image::FromFile(L"pawn.png");
    images[Piece::WN] = Image::FromFile(L"knight.png");
    images[Piece::WB] = Image::FromFile(L"bishop.png");
    images[Piece::WR] = Image::FromFile(L"rook.png");
    images[Piece::WQ] = Image::FromFile(L"queen.png");
    images[Piece::WK] = Image::FromFile(L"king.png");
    images[Piece::BP] = Image::FromFile(L"pawn1.png");
    images[Piece::BN] = Image::FromFile(L"knight1.png");
    images[Piece::BB] = Image::FromFile(L"bishop1.png");
    images[Piece::BR] = Image::FromFile(L"rook1.png");
    images[Piece::BQ] = Image::FromFile(L"queen1.png");
    images[Piece::BK] = Image::FromFile(L"king1.png");

}

void VisitorVisualization::freeImages()
{

    for (auto& kv : images) delete kv.second;
    images.clear();

}

Piece VisitorVisualization::figureToPiece(const Figure& figure) const
{

    switch (figure.getType())
    {

    case FigureType::PAWN: return figure.getColor() == MyColor::WHITE ? Piece::WP : Piece::BP;
    case FigureType::KNIGHT: return figure.getColor() == MyColor::WHITE ? Piece::WN : Piece::BN;
    case FigureType::BISHOP: return figure.getColor() == MyColor::WHITE ? Piece::WB : Piece::BB;
    case FigureType::ROOK: return figure.getColor() == MyColor::WHITE ? Piece::WR : Piece::BR;
    case FigureType::QUEEN: return figure.getColor() == MyColor::WHITE ? Piece::WQ : Piece::BQ;
    case FigureType::KING: return figure.getColor() == MyColor::WHITE ? Piece::WK : Piece::BK;
    default: return Piece::NONE;

    }

}

void VisitorVisualization::drawBoard()
{

    if (!graphics) return;

    for (int currentRowIndex = 0;currentRowIndex < 8;currentRowIndex++)
    {

        for (int currentColIndex = 0;currentColIndex < 8;currentColIndex++)
        {

            Color color = ((currentRowIndex + currentColIndex) % 2 == 0) ? Color(255, 240, 217, 181) : Color(255, 181, 136, 99);
            SolidBrush brush(color);
            graphics->FillRectangle(&brush, currentColIndex * size, currentRowIndex * size, size, size);

        }

    }

}

void VisitorVisualization::drawFigure(const Figure& figure)
{

    if (!graphics) return;

    Piece piece = figureToPiece(figure);
    Image* image = getImage(piece);

    if (!image) return;

    Position position = figure.getPosition();
    graphics->DrawImage(image, position.col * size, position.row * size, size, size);

}


#include "FigureFactory.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Bishop.h"

Figure* FigureFactory::create(FigureType type, MyColor color) 
{

    // single switch to new concrete piece
    switch (type) 
    {

    case FigureType::PAWN: return new Pawn(color);
    case FigureType::KNIGHT: return new Knight(color);
    case FigureType::BISHOP: return new Bishop(color);
    case FigureType::ROOK: return new Rook(color);
    case FigureType::QUEEN: return new Queen(color);
    case FigureType::KING: return new King(color);
    default: return nullptr;

    }

    return nullptr;

}

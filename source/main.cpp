/* main.cpp  —  базови автоматични тестове */

#include <iostream>
#include <cassert>

#include "Board.h"
#include "Move.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

static void resetBoard()
{

    Board& board = Board::instance();
    board.clear();                   
    board.setupInitialPosition();    

}

static int countPieces(Board& board)
{

    int counter = 0;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            if (board.at({ (int8_t)currentRowIndex,(int8_t)currentColIndex })) counter += 1;

        }

    }

    return counter;

}

int main()
{

    Board& board = Board::instance();
    auto pieces = [&] { return countPieces(board); };

    std::cout << "[1] Knight g1f3" << std::endl;
    resetBoard();
    int before = pieces();
    Move k1({ 7,6 }, { 5,5 });                    
    board.applyMove(k1); board.undoMove(k1);
    std::cout << "  before=" << before << "  after=" << pieces() << std::endl;
    assert(pieces() == before);

    std::cerr << "[2] Double‑pawn e2e4" << std::endl;
    resetBoard();
    before = pieces();
    Move dbl({ 6,4 }, { 4,4 }, SpecialMove::DOUBLE_PAWN);
    board.applyMove(dbl); board.undoMove(dbl);
    std::cout << "  before=" << before << "  after=" << pieces() << std::endl;
    assert(pieces() == before);

    std::cout << "[3] En‑passant e5xd6" << std::endl;
    resetBoard();
    before = pieces();

    Move e2e4({ 6,4 }, { 4,4 }, SpecialMove::DOUBLE_PAWN);      
    Move e4e5({ 4,4 }, { 3,4 });                                
    Move d7d5({ 1,3 }, { 3,3 }, SpecialMove::DOUBLE_PAWN);      
    Move ep({ 3,4 }, { 2,3 }, SpecialMove::EN_PASSANT);        

    board.applyMove(e2e4);
    board.applyMove(e4e5);
    board.applyMove(d7d5);
    board.applyMove(ep);

    board.undoMove(ep);
    board.undoMove(d7d5);
    board.undoMove(e4e5);
    board.undoMove(e2e4);

    std::cout << "  before=" << before << "  after=" << pieces() << std::endl;
    assert(pieces() == before);

    std::cout << "[4] Castling O‑O" << std::endl;
    resetBoard();
    before = pieces();
    board.applyMove(Move({ 7,5 }, { 5,7 }));      
    board.applyMove(Move({ 7,6 }, { 5,5 }));      
    Move castleKS({ 7,4 }, { 7,6 }, SpecialMove::CASTLING_KING_SIDE);
    board.applyMove(castleKS);
    board.undoMove(castleKS);
    std::cout << "  before=" << before << "  after=" << pieces() << std::endl;
    assert(pieces() == before);

    std::cout << "[5] Promotion a7a8Q" << std::endl;
    resetBoard();
    before = pieces();
    board.set({ 1,0 }, nullptr);                       
    board.set({ 1,0 }, new Pawn(Color::WHITE));        
    Move promo({ 1,0 }, { 0,0 }, SpecialMove::PROMOTION, FigureType::QUEEN);
    board.applyMove(promo);
    board.undoMove(promo);
    std::cout << "  before=" << before << "  after=" << pieces() << std::endl;
    assert(pieces() == before);

    std::cout << std::endl << "All baseline tests passed" << std::endl;
    return 0;

}

#include <iostream>
#include "Board.h"
#include "Position.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

static void printPositions(const MyVector<Position>& positions) 
{

    for (size_t i = 0; i < positions.size(); i++) 
    {

        char file = char('a' + positions[i].col);
        int  rank = 8 - positions[i].row;
        std::cout << file << rank << " ";

    }

    std::cout << std::endl;

}

int main()
{

    Board& board = Board::instance();
    board.clear();

    {

        std::cout << std::endl << "-- Knight moves from e4 --" << std::endl;
        auto* K = new Knight(Color::WHITE);
        Position e4{ 4,4 };
        board.clear();
        board.set(e4, K);
        printPositions(K->generateMoves(board, e4));
        
    }

    {

        std::cout << std::endl << "-- Bishop moves from d4, blocked b2 own, f6 opp --" << std::endl;
        auto* B = new Bishop(Color::WHITE);
        auto* blocker = new Rook(Color::WHITE);
        auto* attacker = new Rook(Color::BLACK);

        board.clear();
        Position d4{ 4,3 }, b2{ 6,1 }, f6{ 2,5 };
        board.set(d4, B);
        board.set(b2, blocker);
        board.set(f6, attacker);

        printPositions(B->generateMoves(board, d4));

    }

    {

        std::cout << std::endl << "-- Rook moves from d4, blocked d6 own, f4 opp --" << std::endl;
        auto* Rk = new Rook(Color::WHITE);
        auto* pOwn = new Pawn(Color::WHITE);
        auto* pOpp = new Pawn(Color::BLACK);

        board.clear();
        Position d4{ 4,3 }, d6{ 2,3 }, f4{ 4,5 };
        board.set(d4, Rk);
        board.set(d6, pOwn);
        board.set(f4, pOpp);

        printPositions(Rk->generateMoves(board, d4));

    }

    {

        std::cout << std::endl << "-- Queen moves from d4 empty board --" << std::endl;
        auto* Q = new Queen(Color::WHITE);

        board.clear();
        Position d4{ 4,3 };
        board.set(d4, Q);
        printPositions(Q->generateMoves(board, d4));

    }

    {

        std::cout << std::endl << "-- King moves from e4 empty board --" << std::endl;
        auto* Kg = new King(Color::WHITE);

        board.clear();
        Position e4{ 4,4 };
        board.set(e4, Kg);
        printPositions(Kg->generateMoves(board, e4));

    }

    {

        std::cout << std::endl << "-- White pawn from e2 --" << std::endl;
        auto* Pw = new Pawn(Color::WHITE);

        board.clear();
        Position e2{ 6,4 };
        board.set(e2, Pw);
        
        board.set({ 5,5 }, new Pawn(Color::BLACK));
        printPositions(Pw->generateMoves(board, e2));

    }

    {

        std::cout << std::endl << "-- Black pawn from d7 --" << std::endl;
        auto* Pb = new Pawn(Color::BLACK);

        board.clear();
        Position d7{ 1,3 };
        board.set(d7, Pb);
        board.set({ 2,2 }, new Pawn(Color::WHITE));
        printPositions(Pb->generateMoves(board, d7));

    }

    {

        std::cout << std::endl << "-- apply/undo move test --" << std::endl;
        board.clear();
        auto* Rk = new Rook(Color::WHITE);
        Position a1{ 7,0 }, a3{ 5,0 };
        board.set(a1, Rk);

        Move m(a1, a3);
        std::cout << "Before apply, at a3: " << (board.at(a3) == nullptr ? "empty\n" : "occupied\n");
        board.applyMove(m);
        std::cout << "After apply, at a3: " << (board.at(a3) == Rk ? "Rook there\n" : "ERR\n");
        board.undoMove(m);
        std::cout << "After undo, back at a1: " << (board.at(a1) == Rk ? "Rook back\n" : "ERR\n");

    }

    {

        std::cout << std::endl << "-- Attack/Check tests --" << std::endl;
        board.clear();
        
        auto* K = new King(Color::WHITE);
        auto* R = new Rook(Color::BLACK);
        Position e1{ 7,4 }, e8{ 0,4 };
        board.set(e1, K);
        board.set(e8, R);

        std::cout << "isUnderAttack(e1, BLACK)? " << (board.isUnderAttack(e1, Color::BLACK) ? "YES\n" : "NO\n");
        std::cout << "isInCheck(WHITE)? " << (board.isInCheck(Color::WHITE) ? "YES\n" : "NO\n");

    }

    {

        std::cout << std::endl << "-- isLegalMove tests --" << std::endl;
        board.clear();
        auto* K = new King(Color::WHITE);
        auto* R = new Rook(Color::BLACK);
        Position e1{ 7,4 }, e8{ 0,4 };
        board.set(e1, K);
        board.set(e8, R);

        Move escapeKF1(e1, { 7,5 });
        std::cout << "isLegalMove e1->f1? " << (board.isLegalMove(escapeKF1, Color::WHITE) ? "YES\n" : "NO\n");

        Move badK(e1, { 6,4 });
        std::cout << "isLegalMove e1->e2? " << (board.isLegalMove(badK, Color::WHITE) ? "YES\n" : "NO\n");

    }

    {

        std::cout << std::endl << "-- Pawn blocking test --" << std::endl;
        board.clear();
        auto* Pw = new Pawn(Color::WHITE);
        board.set({ 6,4 }, Pw);       
        board.set({ 5,4 }, new Pawn(Color::WHITE));  

        auto vp = Pw->generateMoves(board, { 6,4 });
        std::cout << "White pawn @e2 (blocked ahead) moves: ";
        printPositions(vp);  

    }

    {

        std::cout << std::endl << "-- Pawn capture test --" << std::endl;
        board.clear();
        auto* Pw = new Pawn(Color::WHITE);
        board.set({ 6,4 }, Pw);        
        board.set({ 5,3 }, new Pawn(Color::BLACK));
        board.set({ 5,5 }, new Pawn(Color::BLACK));

        auto vp = Pw->generateMoves(board, { 6,4 });
        std::cout << "White pawn @e2 (can capture): ";
        printPositions(vp);  

    }

    {

        std::cout << std::endl << "-- Castling tests --" << std::endl;
        board.setupInitialPosition();
        
        board.set({ 7,5 }, nullptr);
        board.set({ 7,6 }, nullptr);
        std::cout << "Can castle O-O? " << (board.canCastleKingSide(Color::WHITE) ? "YES\n" : "NO\n");

        Move oO({ 7,4 }, { 7,6 }, SpecialMove::CASTLING_KING_SIDE);

        if (board.isLegalMove(oO, Color::WHITE)) 
        {

            board.applyMove(oO);

            std::cout << "After O-O, King @ " << char('a' + board.peekHistory().move.to.col)
                << (8 - board.peekHistory().move.to.row) << std::endl << "Rook @ f1? "
                << (board.at({ 7,5 }) && board.at({ 7,5 })->symbol() == 'R' ? "YES\n" : "NO\n");

        }

        board.setupInitialPosition();
        board.set({ 7,1 }, nullptr);
        board.set({ 7,2 }, nullptr);
        board.set({ 7,3 }, nullptr);
        std::cout << "Can castle O-O-O? " << (board.canCastleQueenSide(Color::WHITE) ? "YES\n" : "NO\n");

        Move oOO({ 7,4 }, { 7,2 }, SpecialMove::CASTLING_QUEEN_SIDE); 

        if (board.isLegalMove(oOO, Color::WHITE))
        {

            board.applyMove(oOO);

            std::cout << "After O-O-O, King @ " << char('a' + board.peekHistory().move.to.col)
                << (8 - board.peekHistory().move.to.row) << std::endl << "Rook @ d1? "
                << (board.at({ 7,3 }) && board.at({ 7,3 })->symbol() == 'R' ? "YES\n" : "NO\n");

        }

    }

    {

        std::cout << std::endl << "-- En passant tests --" << std::endl;
        board.clear();
        
        auto* wp = new Pawn(Color::WHITE);
        board.set({ 3,4 }, wp);
       
        board.pushHistory(HistoryEntry(Move({ 1,4 }, { 3,4 }, SpecialMove::DOUBLE_PAWN), nullptr));
        
        auto eps = wp->generateMoves(board, { 3,4 });
        std::cout << "En passant from e5: ";
        printPositions(eps);      

        Move ep({ 3,4 }, { 2,3 }, SpecialMove::EN_PASSANT);

        if (board.isLegalMove(ep, Color::WHITE)) 
        {

            board.applyMove(ep);
            std::cout << "Captured pawn at d5? " << (board.at({ 3,3 }) == nullptr ? "YES\n" : "NO\n");

        }

    }

    {

        std::cout << std::endl << "-- Promotion tests --" << std::endl;
        board.clear();
        
        auto* pw = new Pawn(Color::WHITE);
        board.set({ 1,0 }, pw);

        auto promos = pw->generateMoves(board, { 1,0 });
        std::cout << "Pawn promotions from a7: ";
        printPositions(promos);   

        Move pr({ 1,0 }, { 0,0 }, SpecialMove::PROMOTION);
        board.applyMove(pr);
        std::cout << "After promo @a8: " << board.at({ 0,0 })->symbol() << std::endl;   

    }

    std::cout << std::endl << "All figures/board tests done" << std::endl;

    return 0;

}

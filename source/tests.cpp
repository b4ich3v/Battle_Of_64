#include <iostream>
#include <random>
#include "Board.h"
#include "Rook.h"
#include "Move.h"
#include "AIPlayer.h"
#include "Pawn.h"
#include <iostream>
#include <random>
#include <vector>                
#include "Board.h"
#include "Move.h"
#include "Pawn.h"
#include "King.h"                
#include "Queen.h"
#include "BinaryWriter.h"
#include "BinaryReader.h"        

#define EXPECT(expr, msg)                                                   \
    do { if (!(expr)) {                                                     \
        std::cerr << "[FAIL] " << msg << "  (" #expr ")" << std::endl;      \
        return false; } } while (0)

#define RUN(test)                                                           \
    do { std::cout << "[RUN ] " #test "\n";                                 \
         if (test()) std::cout << "[PASS] " #test "\n\n";                   \
         else return EXIT_FAILURE; } while (0)

#define ASSERT(expr,msg) \
    do{ if(!(expr)){ std::cerr<<"[FAIL] "<<msg<<"\n"; return false;}}while(0)
#define RUN(test) \
    do{ std::cout<<"[RUN ] "<<#test<<"\n"; \
        if(test()) std::cout<<"[PASS] "<<#test<<"\n\n"; \
        else return EXIT_FAILURE; }while(0)


static Position sq(const char* str)
{

    return { int8_t(8 - (str[1] - '0')), int8_t(str[0] - 'a') };

}

bool t_init()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    int counter = 0;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++) 
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            if (board.at({ currentRowIndex,currentColIndex })) counter += 1;

        }

    }

    EXPECT(counter == 32, "Initial board must contain 32 pieces");

    return true;

}

bool t_enpass()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    Move e2e4(sq("e2"), sq("e4"), SpecialMove::DOUBLE_PAWN);   
    board.applyMove(e2e4);

    Move d7d5(sq("d7"), sq("d5"), SpecialMove::DOUBLE_PAWN);   
    board.applyMove(d7d5);

    Move enp(sq("e4"), sq("d5"), SpecialMove::EN_PASSANT);

    EXPECT(board.isLegalMove(enp, MyColor::WHITE), "en passant must be legal");
    board.applyMove(enp);
    EXPECT(board.at(sq("d5")) && board.at(sq("d5"))->symbol() == 'P', "White pawn must land on d5");

    return true;

}

bool t_castle_undo()
{

    Board& board = Board::instance();
    board.setupInitialPosition();
    board.set(sq("f1"), nullptr);  
    board.set(sq("g1"), nullptr);

    Move castle(sq("e1"), sq("g1"), SpecialMove::CASTLING_KING_SIDE);
    EXPECT(board.isLegalMove(castle, MyColor::WHITE), "castle must be legal");
    board.applyMove(castle);
    EXPECT(board.at(sq("g1"))->symbol() == 'K', "King on g1 after castling");

    board.undoMove(castle);
    EXPECT(board.at(sq("e1"))->symbol() == 'K', "King back on e1 after undo");

    return true;

}

bool test_promotion_and_undo()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    if (Figure* figure = board.at(sq("a7"))) { delete figure; board.set(sq("a7"), nullptr); }
    if (Figure* figure = board.at(sq("a8"))) { delete figure; board.set(sq("a8"), nullptr); }

    Pawn* wp = new Pawn(MyColor::WHITE);
    wp->setPosition(sq("a7"));              
    board.set(sq("a7"), wp);

    Move promo(sq("a7"), sq("a8"), SpecialMove::PROMOTION, FigureType::QUEEN);

    ASSERT(board.isLegalMove(promo, MyColor::WHITE), "promotion must be legal");
    board.applyMove(promo);
    ASSERT(board.at(sq("a8")) && board.at(sq("a8"))->symbol() == 'Q', "Queen should be on a8");

    board.undoMove(promo);
    ASSERT(board.at(sq("a7")) && board.at(sq("a7"))->symbol() == 'P', "Pawn should be back on a7");
    ASSERT(board.at(sq("a8")) == nullptr, "a8 must be empty after undo");

    return true;

}

bool t_apply_undo_identity()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    auto moves = board.generateAllLegalMoves(MyColor::WHITE);
    EXPECT(moves.size() > 0, "White must have legal moves");

    Move move = moves[0];
    Position from = move.from;
    char sym = board.at(from)->symbol();

    board.applyMove(move);
    board.undoMove(move);

    EXPECT(board.at(from) && board.at(from)->symbol() == sym, "Board identical after apply+undo");

    return true;

}

bool t_fools_mate()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    board.applyMove(Move(sq("f2"), sq("f3")));   
    board.applyMove(Move(sq("e7"), sq("e5")));   
    board.applyMove(Move(sq("g2"), sq("g4")));   
    board.applyMove(Move(sq("d8"), sq("h4")));   

    EXPECT(!board.hasLegalMoves(MyColor::WHITE), "White must have no legal moves (mate)");
    EXPECT(board.isInCheck(MyColor::WHITE), "White must be in check (mate)");

    return true;

}

bool t_stalemate()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Position position{ int8_t(currentRowIndex), int8_t(currentColIndex) };
            if (Figure* figure = board.at(position)) { delete figure; board.set(position, nullptr); }

        }

    }

    board.set(sq("h8"), new King(MyColor::BLACK));
    board.set(sq("g6"), new King(MyColor::WHITE));
    board.set(sq("f7"), new Queen(MyColor::WHITE));

    EXPECT(!board.hasLegalMoves(MyColor::BLACK), "Black should have no legal moves (stalemate)");
    EXPECT(!board.isInCheck(MyColor::BLACK), "Black king is not in check (stalemate)");

    return true;

}

bool t_apply_undo_stress()
{

    Board& board = Board::instance();
    board.setupInitialPosition();
    std::mt19937 rng(123);
    MyColor turn = MyColor::WHITE;          

    for (int i = 0; i < 100; i++)
    {

        auto moves = board.generateAllLegalMoves(turn);
        ASSERT(!moves.empty(), "must have moves during stress-loop");

        size_t idx = rng() % moves.size();   
        Move   m = moves[idx];
        board.applyMove(m);
        board.undoMove(m);

        turn = oppositeColor(turn);     

    }

    int counter = 0;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++) 
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            if (board.at({ currentRowIndex,currentColIndex })) ++counter;

        }

    }
        
    EXPECT(counter == 32, "32 pieces should remain after stress loop");

    return true;

}

bool t_save_load_roundtrip()
{

    Board& board = Board::instance();
    board.setupInitialPosition();
    board.applyMove(Move(sq("e2"), sq("e4")));
    board.applyMove(Move(sq("e7"), sq("e5")));

    char before[64];

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Figure* figure = board.at({ currentRowIndex,currentColIndex });
            before[currentRowIndex * 8 + currentColIndex] = figure ? figure->symbol() : '.';

        }

    }

    const char* path = "test_save.sav";
    BinaryWriter bw;
    ASSERT(bw.open(path), "cannot open file for writing");
    board.serialize(bw);
    bw.close();

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Position position{ int8_t(currentRowIndex), int8_t(currentColIndex) };
            if (Figure* f = board.at(position)) { delete f; board.set(position, nullptr); }

        }

    }

    BinaryReader binaryReader;
    ASSERT(binaryReader.open(path), "cannot open file for reading");
    board.deserialize(binaryReader);
    binaryReader.close();
    std::remove(path);                      

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Figure* figure = board.at({ currentRowIndex,currentColIndex });
            char sym = figure ? figure->symbol() : '.';
            EXPECT(sym == before[currentRowIndex * 8 + currentColIndex], "mismatch after load");

        }

    }

    return true;

}

bool t_castle_illegal_due_attack()
{

    Board& board = Board::instance();
    board.setupInitialPosition();

    if (Figure* figure = board.at(sq("f2"))) { delete figure; board.set(sq("f2"), nullptr); }
    if (Figure* figure = board.at(sq("g2"))) { delete figure; board.set(sq("g2"), nullptr); }
    if (Figure* figure = board.at(sq("f8"))) { delete figure; board.set(sq("f8"), nullptr); }

    board.set(sq("f3"), new Rook(MyColor::BLACK));

    Move castle(sq("e1"), sq("g1"), SpecialMove::CASTLING_KING_SIDE);
    EXPECT(!board.isLegalMove(castle, MyColor::WHITE),"Castling through an attacked square must be illegal");

    return true;

}

bool t_enpass_late_illegal()
{

    Board& board = Board::instance(); board.setupInitialPosition();
    board.applyMove(Move(sq("e2"), sq("e4")));   
    board.applyMove(Move(sq("a7"), sq("a6")));   

    Move enp(sq("e4"), sq("d5"), SpecialMove::EN_PASSANT);
    EXPECT(!board.isLegalMove(enp, MyColor::WHITE), "Late en passant must be illegal after one quiet move");

    return true;

}

int main()
{

    RUN(t_init);
    RUN(t_enpass);
    RUN(t_castle_undo);
    RUN(test_promotion_and_undo);
    RUN(t_apply_undo_identity);
    RUN(t_fools_mate);
    RUN(t_stalemate);
    RUN(t_save_load_roundtrip);
    RUN(t_apply_undo_stress);
    RUN(t_enpass_late_illegal);
    RUN(t_castle_illegal_due_attack);

    std::cout << "All tests passed!" << std::endl;
    return EXIT_SUCCESS;

}

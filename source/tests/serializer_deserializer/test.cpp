#include <iostream>
#include "Board.h"
#include "TextBoardIO.h"
#include "BinaryBoardIO.h"
#include "Position.h"
#include "Figure.h"

int main() 
{

    Board& board = Board::instance();

    {

        std::cout << std::endl << "-- Text serialization --" << std::endl;
        
        board.setupInitialPosition();
        board.set({ 7,4 }, nullptr);         
        board.set({ 0,0 }, nullptr);         

        TextBoardIO file("test_board.txt");
        file.save(board);
        std::cout << "Saved to test_board.txt" << std::endl;

        board.clear();
        std::cout << "After clear, at e1: "
            << (board.at({ 7,4 }) == nullptr ? "empty\n" : "occupied\n")
            << "             at a8: " << (board.at({ 0,0 }) == nullptr ? "empty\n" : "occupied\n");
        
        file.load(board);
        std::cout << "Loaded from test_board.txt" << std::endl;
        std::cout << "After load, at e1: " << (board.at({ 7,4 }) == nullptr ? "empty\n" : "ERR\n") 
            << "             at a8: " << (board.at({ 0,0 }) == nullptr ? "empty\n" : "ERR\n");

    }

    {

        std::cout << std::endl << "-- Binary serialization --" << std::endl;
        
        board.clear();
        board.setupInitialPosition();
        board.set({ 7,1 }, nullptr);

        BinaryBoardIO file("test_board.dat");
        file.save(board);
        std::cout << "Saved to test_board.dat" << std::endl;

        board.clear();
        std::cout << "After clear, at b1: " << (board.at({ 7,1 }) == nullptr ? "empty\n" : "occupied\n");

        file.load(board);
        std::cout << "Loaded from test_board.dat" << std::endl;
        std::cout << "After load, at b1: " << (board.at({ 7,1 }) == nullptr ? "empty\n" : "ERR\n");

    }

    std::cout << std::endl <<"All serialization tests done" << std::endl;

    return 0;

}


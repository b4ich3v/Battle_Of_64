#include <iostream>
#include "Board.h"
#include "Figure.h"
#include "Pawn.h"

int main() 
{

    Board board;
    Pawn wp(Color::WHITE);
    Position e2(6, 4);
    board.set(e2, &wp);
    auto moves = wp.generateMoves(board, e2);

    for (size_t i = 0; i < moves.size(); ++i)
    {

        Position m = moves[i];
        char file = 'a' + m.col;
        int rank = 8 - m.row;
        std::cout << file << rank << " ";

    }

    std::cout << std::endl;
    return 0;

}

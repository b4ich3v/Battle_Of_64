#include "HumanPlayer.h"
#include <cctype>
#include <iostream>

namespace 
{

    bool parseSquare(const MyString& token, Position& outPosition)
    {

        if (token.size() != 2) return false;

        char f = std::tolower(token[0]);
        char r = token[1];

        if (f < 'a' || f > 'h' || r < '1' || r > '8') return false;

        outPosition.col = (int8_t)(f - 'a');
        outPosition.row = (int8_t)(8 - (r - '0'));

        return true;

    }

    FigureType promoFromChar(char ch)
    {

        switch (std::tolower(ch)) 
        {

        case 'q': return FigureType::QUEEN;
        case 'r': return FigureType::ROOK;
        case 'b': return FigureType::BISHOP;
        case 'n': return FigureType::KNIGHT;
        default: return FigureType::NONE;

        }

    }

} 

HumanPlayer::HumanPlayer(const MyString& name): 
    Player(name) {}

Move HumanPlayer::requestMove(Board& board, Color side)
{

    while (true) 
    {

        std::cout << (side == Color::WHITE ? "White" : "Black") << " > ";
        MyString input;
        std::cin >> input;

        if (!std::cin) std::exit(0);

        if (input == "O-O" || input == "o-o" || input == "0-0") 
        {

            int8_t row = (side == Color::WHITE ? 7 : 0);
            Move move(Position(row, 4), Position(row, 6), SpecialMove::CASTLING_KING_SIDE);

            if (!board.isLegalMove(move, side))
            {

                std::cout << "Illegal move – try again" << std::endl;
                continue;

            }

            return move;
                
        }

        if (input == "O-O-O" || input == "o-o-o" || input == "0-0-0") 
        {

            int8_t row = (side == Color::WHITE ? 7 : 0);
            Move move(Position(row, 4), Position(row, 2), SpecialMove::CASTLING_QUEEN_SIDE);

            if (!board.isLegalMove(move, side))
            {

                std::cout << "Illegal move – try again" << std::endl;
                continue;

            }

            return move;

        }

        if (input.size() < 4)
        {

            std::cout << "Bad format" << std::endl; 
            continue; 

        }

        MyString fromStr = input.substr(0, 2);
        MyString toStr = input.substr(2, 2);

        Position from;
        Position to;

        if (!parseSquare(fromStr, from) || !parseSquare(toStr, to)) 
        {

            std::cout << "Bad squares" << std::endl;
            continue;

        }

        FigureType promote = FigureType::NONE;
        SpecialMove specialMove = SpecialMove::NORMAL;

        if (input.size() == 5) 
        {

            promote = promoFromChar(input[4]);

            if (promote == FigureType::NONE) 
            {

                std::cout << "Bad promotion piece" << std::endl;
                continue;

            }

            specialMove = SpecialMove::PROMOTION;

        }

        if (specialMove == SpecialMove::NORMAL &&
            std::abs(from.row - to.row) == 2) 
        {

            specialMove = SpecialMove::DOUBLE_PAWN;

        }
           
        Move move(from, to, specialMove, promote);

        if (!board.isLegalMove(move, side)) 
        {

            std::cout << "Illegal move – try again" << std::endl;
            continue;

        }

        return move;

    }

}

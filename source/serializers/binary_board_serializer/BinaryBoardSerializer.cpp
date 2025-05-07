#include <fstream>
#include "BinaryBoardSerializer.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

BinaryBoardSerializer::BinaryBoardSerializer(const MyString& fileName):
    BoardSerializer(fileName) {}

void BinaryBoardSerializer::save(const Board& board) const
{

    std::ofstream file(fileName.getData(), std::ios::binary);

    if (!file.is_open()) return;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            char symbol = board.at({ currentRowIndex,currentColIndex }) ? board.at({ currentRowIndex,currentColIndex })->symbol() : '.';
            file.write(&symbol, 1);

        }

    }

    file.close();

}

void BinaryBoardSerializer::load(Board& board) const 
{

    std::ifstream file(fileName.getData(), std::ios::binary);

    if (!file.is_open()) return;

    board.clear();

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++) 
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            board.set({ currentRowIndex,currentColIndex }, nullptr);

        }

    }
        
    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++) 
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++) 
        {

            char ch = 'a';
            file.read(&ch, 1);

            if (ch == '.') continue;

            Color color = (ch >= 'A' && ch <= 'Z') ? Color::WHITE : Color::BLACK;
            char lower = (ch >= 'A' && ch <= 'Z') ? (ch - 'A' + 'a') : ch;
            Figure* figure = nullptr;

            switch (lower) 
            {

            case 'p': figure = new Pawn(color); break;
            case 'r': figure = new Rook(color); break;
            case 'n': figure = new Knight(color); break;
            case 'b': figure = new Bishop(color); break;
            case 'q': figure = new Queen(color); break;
            case 'k': figure = new King(color); break;
            default:  break;

            }

            board.set({ currentRowIndex,currentColIndex }, figure);

        }

    }

    file.close();

}

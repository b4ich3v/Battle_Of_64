#include <fstream>
#include "TextBoardIO.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"

TextBoardIO::TextBoardIO(const MyString& fileName):
    BoardSerializer(fileName) {}

void TextBoardIO::save(const Board& board) const
{

    std::ofstream file(fileName.getData());

    if (!file.is_open()) return;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            auto* currentFigure = board.at({ currentRowIndex,currentColIndex });
            file << (currentFigure ? currentFigure->symbol() : '.');

        }

        file << std::endl;

    }

    file.close();

}

void TextBoardIO::load(Board& board) const
{

    std::ifstream file(fileName.getData());

    if (!file.is_open()) return;

    board.clear();

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++) 
        {

            board.set({ currentRowIndex,currentColIndex }, nullptr);

        }

    }
        
    char buffer[16];  
    int currentRowIndex = 0;

    while (currentRowIndex < 8 && file.getline(buffer, sizeof(buffer))) 
    {

        for (int currentColIndex = 0; currentColIndex < 8 && buffer[currentColIndex] != '\0'; ++currentColIndex)
        {

            char ch = buffer[currentColIndex];
            if (ch == '.') continue;

            Color color = (ch >= 'A' && ch <= 'Z') ? Color::WHITE: Color::BLACK;
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
            default: break;

            }

            board.set({ currentRowIndex, currentColIndex }, figure);

        }

        currentRowIndex++;
            
    }

    file.close();

}

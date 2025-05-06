#include "Board.h"

HistoryEntry::HistoryEntry():
    move(Position{ 0,0 }, Position{ 0,0 }), captured(nullptr) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured):
    move(move), captured(captured) {}

Board::Board(): table(), history() 
{

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        MyVector<Figure*> currentRow;

        for (int currentColIndex = 0; currentColIndex < 8; ++currentColIndex)
        {

            currentRow.push_back(nullptr);

        }

        table.push_back(currentRow);

    }

}

bool Board::isValid(const Position& position) const
{

    return position.row >= 0 && position.row < 8
        && position.col >= 0 && position.col < 8;

}

Figure* Board::at(const Position& position) const
{

    if (!isValid(position)) throw std::out_of_range("Position out of range");
    return table[position.row][position.col];

}

void Board::set(const Position& position, Figure* figure)
{

    if (!isValid(position)) throw std::out_of_range("Position out of range");
    table[position.row][position.col] = figure;

}

void Board::applyMove(const Move& move)
{

    Figure* figure = at(move.from);
    Figure* captured = at(move.to);

    set(move.to, figure);
    set(move.from, nullptr);
    pushHistory(HistoryEntry(move, captured));

}

void Board::undoMove(const Move& move) 
{

    auto entry = popHistory();
    set(entry.move.from, at(entry.move.to));
    set(entry.move.to, entry.captured);

}

void Board::pushHistory(HistoryEntry entry)
{

    history.push_back(entry);

}

bool Board::canCastleKingSide(Color color) const 
{

    int row = (color == Color::WHITE ? 7 : 0);
    
    Figure* king = at(Position(row, 4));
    Figure* rook = at(Position(row, 7));

    if (!king || !rook) return false;
    if (king->getType() != FigureType::KING || rook->getType() != FigureType::ROOK) return false;
    if (at(Position(row, 5)) || at(Position(row, 6))) return false;

    return true;

}

bool Board::canCastleQueenSide(Color color) const 
{

    int row = (color == Color::WHITE ? 7 : 0);
    
    Figure* king = at(Position(row, 4));
    Figure* rook = at(Position(row, 0));

    if (!king || !rook) return false;
    if (king->getType() != FigureType::KING || rook->getType() != FigureType::ROOK) return false;
    if (at(Position(row, 1)) || at(Position(row, 2)) || at(Position(row, 3))) return false;

    return true;

}

bool Board::isUnderAttack(const Position& position, Color attacker) const 
{
 
    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Figure* currentFigure = at({ currentRowIndex,currentColIndex });

            if (currentFigure && currentFigure->getColor() == attacker)
            {

                auto move = currentFigure->generateMoves(*this, { currentRowIndex,currentColIndex });

                for (size_t i = 0; i < move.size(); i++)
                {

                    if (move[i] == position) return true;

                }

            }

        }

    }

    return false;

}

bool Board::isInCheck(Color c) const 
{
    
    Position kingPosition{ -1,-1 };

    for (int currentRowIndex = 0; currentRowIndex < 8; ++currentRowIndex) 
    {

        for (int currentColIndex = 0; currentColIndex < 8; ++currentColIndex)
        {

            if (Figure* currentFigure = at({ currentRowIndex,currentColIndex }))
            {

                if (currentFigure->getColor() == c && currentFigure->symbol() == (c == Color::WHITE ? 'K' : 'k'))
                {

                    kingPosition = { currentRowIndex,currentColIndex };

                }

            }
                   
        }   

    }
        
    if (kingPosition.row < 0) throw std::runtime_error("isInCheck: king not found on board");
        
    return isUnderAttack(kingPosition, oppositeColor(c));

}

bool Board::isLegalMove(const Move& move, Color moverSide)
{
    
    if (Figure* currentFigure = at(move.from)) 
    {

        if (currentFigure->getColor() != moverSide) return false;

    }
    else return false;

    auto pseudo = at(move.from)->generateMoves(*this, move.from);
    bool found = false;

    for (size_t i = 0; i < pseudo.size(); i++)
    {

        if (pseudo[i] == move.to) { found = true; break; }

    }

    if (!found) return false;

    applyMove(move);
    bool inCheck = isInCheck(moverSide);
    undoMove(move);

    return !inCheck;

}

HistoryEntry Board::peekHistory() const 
{

    size_t sizeOfHistory = history.size();
    if (sizeOfHistory == 0) throw std::runtime_error("No history available");
    return history[sizeOfHistory - 1];

}

HistoryEntry Board::popHistory()
{

    size_t sizeOfHistory = history.size();
    if (sizeOfHistory == 0) throw std::runtime_error("No history to undo");

    HistoryEntry entry = history[sizeOfHistory - 1];
    history.pop_back();

    return entry;

}

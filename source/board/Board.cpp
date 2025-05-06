#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "Board.h"

HistoryEntry::HistoryEntry():
    move(Position{ 0,0 }, Position{ 0,0 }), captured(nullptr) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured):
    move(move), captured(captured) {}

HistoryEntry::HistoryEntry(const Move & move, Figure* captured,
    const MyVector<bool>&prevKing, const MyVector<bool>&prevQueen): 
    move(move), captured(captured), 
    castleKingSide(prevKing), castleQueenSide(prevQueen) {}

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

Board::~Board() 
{

    free();

}

Board& Board::instance()
{

    static Board inst;
    return inst;

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

void Board::setupInitialPosition() 
{
    
    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++) 
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            set({ currentRowIndex,currentColIndex }, nullptr);

        }
        
    }
        
    history.clear();
    castleKS.clear();
    castleQS.clear();

    castleKS.push_back(true);
    castleKS.push_back(true);
    castleQS.push_back(true);
    castleQS.push_back(true);

    set({ 7,0 }, new Rook(Color::WHITE));
    set({ 7,1 }, new Knight(Color::WHITE));
    set({ 7,2 }, new Bishop(Color::WHITE));
    set({ 7,3 }, new Queen(Color::WHITE));
    set({ 7,4 }, new King(Color::WHITE));
    set({ 7,5 }, new Bishop(Color::WHITE));
    set({ 7,6 }, new Knight(Color::WHITE));
    set({ 7,7 }, new Rook(Color::WHITE));

    for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
    {

        set({ 6,currentColIndex }, new Pawn(Color::WHITE));

    }
        
    set({ 0,0 }, new Rook(Color::BLACK));
    set({ 0,1 }, new Knight(Color::BLACK));
    set({ 0,2 }, new Bishop(Color::BLACK));
    set({ 0,3 }, new Queen(Color::BLACK));
    set({ 0,4 }, new King(Color::BLACK));
    set({ 0,5 }, new Bishop(Color::BLACK));
    set({ 0,6 }, new Knight(Color::BLACK));
    set({ 0,7 }, new Rook(Color::BLACK));

    for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++) 
    {

        set({ 1,currentColIndex }, new Pawn(Color::BLACK));

    }

}

void Board::free()
{
    
    for (size_t currentRowIndex = 0; currentRowIndex < table.size(); currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < table[currentRowIndex].size(); ++currentColIndex)
        {

            delete table[currentRowIndex][currentColIndex];
            table[currentRowIndex][currentColIndex] = nullptr;

        }

    }
    
    for (size_t i = 0; i < history.size(); ++i) 
    {

        if (history[i].captured) 
        {

            delete history[i].captured;
            history[i].captured = nullptr;

        }

    }
    
    table.clear();
    history.clear();
    castleKS.clear();
    castleQS.clear();

}

void Board::applyMove(const Move& move) 
{
    
    HistoryEntry entry(move, at(move.to), castleKS, castleQS);

    move.execute(*this);
    pushHistory(entry);

}

void Board::undoMove(const Move&) 
{
    
    auto entry = popHistory();

    castleKS = entry.castleKingSide;
    castleQS = entry.castleQueenSide;

    entry.move.undo(*this);

}

void Board::pushHistory(HistoryEntry entry)
{

    history.push_back(entry);

}

bool Board::canCastleKingSide(Color color) const 
{

    int index = (int)(color);
    if (!castleKS[index]) return false;

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

    int index = (int)(color);
    if (!castleQS[index]) return false;

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

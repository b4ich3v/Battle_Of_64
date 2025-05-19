#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "Board.h"

HistoryEntry::HistoryEntry() :
    move(Position{ 0,0 }, Position{ 0,0 }), captured(nullptr) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured) :
    move(move), captured(captured) {}

HistoryEntry::HistoryEntry(const Move& move, Figure* captured,
    const MyVector<bool>& prevKing, const MyVector<bool>& prevQueen) :
    move(move), captured(captured),
    castleKingSide(prevKing), castleQueenSide(prevQueen) {}

Board::Board() : table(), history()
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

    if (figure) figure->setPosition(position);

}

void Board::accept(Visitor& visitor) const
{

    visitor.visit(*this);

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            if (Figure* currentFigure = at({ currentRowIndex,currentColIndex })) currentFigure->accept(visitor);

        }

    }

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

    set({ 7,0 }, new Rook(MyColor::WHITE));
    set({ 7,1 }, new Knight(MyColor::WHITE));
    set({ 7,2 }, new Bishop(MyColor::WHITE));
    set({ 7,3 }, new Queen(MyColor::WHITE));
    set({ 7,4 }, new King(MyColor::WHITE));
    set({ 7,5 }, new Bishop(MyColor::WHITE));
    set({ 7,6 }, new Knight(MyColor::WHITE));
    set({ 7,7 }, new Rook(MyColor::WHITE));

    for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
    {

        set({ 6,currentColIndex }, new Pawn(MyColor::WHITE));

    }

    set({ 0,0 }, new Rook(MyColor::BLACK));
    set({ 0,1 }, new Knight(MyColor::BLACK));
    set({ 0,2 }, new Bishop(MyColor::BLACK));
    set({ 0,3 }, new Queen(MyColor::BLACK));
    set({ 0,4 }, new King(MyColor::BLACK));
    set({ 0,5 }, new Bishop(MyColor::BLACK));
    set({ 0,6 }, new Knight(MyColor::BLACK));
    set({ 0,7 }, new Rook(MyColor::BLACK));

    for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
    {

        set({ 1,currentColIndex }, new Pawn(MyColor::BLACK));

    }

}

void Board::free()
{

    for (size_t currentRowIndex = 0; currentRowIndex < table.size(); currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < table[currentRowIndex].size(); currentColIndex++)
        {

            delete table[currentRowIndex][currentColIndex];
            table[currentRowIndex][currentColIndex] = nullptr;

        }

    }

    table.clear();
    history.clear();
    castleKS.clear();
    castleQS.clear();

}

void Board::clear()
{

    free();
    table.clear();

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        MyVector<Figure*> currentRow;

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            currentRow.push_back(nullptr);

        }

        table.push_back(currentRow);

    }

    history.clear();
    castleKS.clear();
    castleQS.clear();

    castleKS.push_back(true);
    castleKS.push_back(true);
    castleQS.push_back(true);
    castleQS.push_back(true);

}

void Board::applyMove(const Move& move)
{

    Figure* captured = nullptr;

    if (move.getSpecial() == SpecialMove::EN_PASSANT)
    {

        MyColor moverColor = at(move.from)->getColor();
        int8_t dir = (moverColor == MyColor::WHITE ? +1 : -1);
        Position capPosition{ (int8_t)(move.to.row + dir), move.to.col };
        captured = at(capPosition);

    }
    else
    {

        captured = at(move.to);

    }

    HistoryEntry entry(move, captured, castleKS, castleQS);
    pushHistory(entry);
    move.execute(*this);

}

void Board::undoMove(const Move&)
{

    auto entry = popHistory();
    entry.move.undo(*this);

}

void Board::pushHistory(HistoryEntry entry)
{

    history.push_back(entry);

}

bool Board::canCastleKingSide(MyColor color) const
{

    int index = (int)(color);
    if (!castleKS[index]) return false;

    int row = (color == MyColor::WHITE ? 7 : 0);

    Figure* king = at(Position(row, 4));
    Figure* rook = at(Position(row, 7));

    if (!king || !rook) return false;
    if (king->getType() != FigureType::KING || rook->getType() != FigureType::ROOK) return false;
    if (at(Position(row, 5)) || at(Position(row, 6))) return false;

    return true;

}

bool Board::canCastleQueenSide(MyColor color) const
{

    int index = (int)(color);
    if (!castleQS[index]) return false;

    int row = (color == MyColor::WHITE ? 7 : 0);

    Figure* king = at(Position(row, 4));
    Figure* rook = at(Position(row, 0));

    if (!king || !rook) return false;
    if (king->getType() != FigureType::KING || rook->getType() != FigureType::ROOK) return false;
    if (at(Position(row, 1)) || at(Position(row, 2)) || at(Position(row, 3))) return false;

    return true;

}

bool Board::isUnderAttack(const Position& position, MyColor attacker) const
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

                    if (move[i].to == position) return true;

                }

            }

        }

    }

    return false;

}

bool Board::isInCheck(MyColor color) const
{

    Position kingPosition{ -1,-1 };

    for (int currentRowIndex = 0; currentRowIndex < 8; ++currentRowIndex)
    {

        for (int currentColIndex = 0; currentColIndex < 8; ++currentColIndex)
        {

            if (Figure* currentFigure = at({ currentRowIndex,currentColIndex }))
            {

                if (currentFigure->getColor() == color &&
                    currentFigure->symbol() == (color == MyColor::WHITE ? 'K' : 'k'))
                {

                    kingPosition = { currentRowIndex,currentColIndex };

                }

            }

        }

    }

    if (kingPosition.row < 0) throw std::runtime_error("isInCheck: king not found on board");

    return isUnderAttack(kingPosition, oppositeColor(color));

}

bool Board::isEnPassantSquare(const Position& cap, MyColor pawnColor) const
{

    if (history.size() == 0) return false;

    const HistoryEntry& last = history[history.size() - 1];
    const Move& move = last.move;

    if (move.special != SpecialMove::DOUBLE_PAWN) return false;

    int8_t targetRow = (pawnColor == MyColor::WHITE ? 3 : 4);

    return (cap.row == targetRow && cap.col == move.to.col);

}

MyVector<Move> Board::generateAllLegalMoves(MyColor side)
{

    MyVector<Move> allMoves;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Figure* currentFigure = table[currentRowIndex][currentColIndex];
            if (!currentFigure || currentFigure->getColor() != side) continue;

            Position from{ (int8_t)currentRowIndex,(int8_t)currentColIndex };
            MyVector<Move> moves = currentFigure->generateMoves(*this, from);

            for (size_t i = 0; i < moves.size(); i++)
            {

                if (isLegalMove(moves[i], side))
                    allMoves.push_back(moves[i]);

            }

        }

    }

    return allMoves;

}

bool Board::isLegalMove(const Move& move, MyColor side)
{

    applyMove(move);
    bool isValid = !isInCheck(side);
    undoMove(move);

    return isValid;

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

void Board::serialize(Writer& writer) const
{

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Piece piece = figureToPiece(at({ (int8_t)currentRowIndex,(int8_t)currentColIndex }));
            writer.write(&piece, 1);

        }

    }

}

void Board::deserialize(Reader& reader)
{

    clear();

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
        {

            Piece piece; reader.read(&piece, 1);
            if (piece == Piece::NONE) continue;

            Figure* currentFigure = createFigureFromPiece(piece);
            set(Position{ (int8_t)(currentRowIndex), (int8_t)(currentColIndex) }, currentFigure);

        }

    }

}

Piece Board::figureToPiece(const Figure* figure)
{

    if (!figure) return Piece::NONE;

    switch (figure->getType()) 
    {

    case FigureType::PAWN: return (figure->getColor() == MyColor::WHITE) ? Piece::WP : Piece::BP;
    case FigureType::KNIGHT: return (figure->getColor() == MyColor::WHITE) ? Piece::WN : Piece::BN;
    case FigureType::BISHOP: return (figure->getColor() == MyColor::WHITE) ? Piece::WB : Piece::BB;
    case FigureType::ROOK: return (figure->getColor() == MyColor::WHITE) ? Piece::WR : Piece::BR;
    case FigureType::QUEEN: return (figure->getColor() == MyColor::WHITE) ? Piece::WQ : Piece::BQ;
    case FigureType::KING: return (figure->getColor() == MyColor::WHITE) ? Piece::WK : Piece::BK;
    default: return Piece::NONE;

    }

}

Figure* Board::createFigureFromPiece(Piece piece)
{

    MyColor color = (piece <= Piece::WK) ? MyColor::WHITE : MyColor::BLACK;

    switch (piece) 
    {

    case Piece::WP: case Piece::BP: return new Pawn(color);
    case Piece::WN: case Piece::BN: return new Knight(color);
    case Piece::WB: case Piece::BB: return new Bishop(color);
    case Piece::WR: case Piece::BR: return new Rook(color);
    case Piece::WQ: case Piece::BQ: return new Queen(color);
    case Piece::WK: case Piece::BK: return new King(color);
    default: return nullptr;

    }

}



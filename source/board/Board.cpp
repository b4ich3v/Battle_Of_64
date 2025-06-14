#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "Board.h"
#include "Types.h"
#include "FigureFactory.h"

Board::Board(): table(), history() // build empty 8×8 grid (nullptrs)
{

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        MyVector<Figure*> currentRow;

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

    // Meyers singleton-one board for entire program
    static Board inst;
    return inst;

}

bool Board::isValid(const Position& position) const
{

    return position.row >= 0 && position.row < ROWS_COUNT
        && position.col >= 0 && position.col < COLS_COUNT;

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

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
        {

            if (Figure* currentFigure = at({ currentRowIndex,currentColIndex })) currentFigure->accept(visitor);

        }

    }

}

void Board::setupInitialPosition() // clear board and place standard start pieces
{

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

    for (size_t currentColIndex = 0; currentColIndex < BOARD_SIZE; currentColIndex++) 
    {

        Figure* figure = FigureFactory::create(backrank[currentColIndex], MyColor::WHITE);
        set({ WHITE_BACK_ROW, int8_t(currentColIndex) }, figure);

    }

    for (size_t currentColIndex = 0; currentColIndex < BOARD_SIZE; currentColIndex++) 
    {

        set({ WHITE_PAWN_ROW, int8_t(currentColIndex) }, FigureFactory::create(FigureType::PAWN, MyColor::WHITE));

    }

    for (size_t currentColIndex = 0; currentColIndex < BOARD_SIZE; currentColIndex++) 
    {

        Figure* figure = FigureFactory::create(backrank[currentColIndex], MyColor::BLACK);
        set({ BLACK_BACK_ROW, int8_t(currentColIndex) }, figure);

    }
    
    for (size_t currentColIndex = 0; currentColIndex < BOARD_SIZE; currentColIndex++) 
    {

        set({ BLACK_PAWN_ROW, int8_t(currentColIndex) }, FigureFactory::create(FigureType::PAWN, MyColor::BLACK));

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

    for (size_t i = 0; i < history.size(); i++)
    {

        HistoryEntry& currentHistory = history[i];
        delete currentHistory.captured;
        currentHistory.captured = nullptr;

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

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        MyVector<Figure*> currentRow;

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

void Board::applyMove(const Move& move) // execute move, update castle rights, push HistoryEntry
{

    Figure* captured = nullptr;

    if (move.getSpecial() == SpecialMove::EN_PASSANT)
    {

        MyColor moverColor = at(move.from)->getColor();
        int8_t dir = (moverColor == MyColor::WHITE ? EP_DIR_BLACK : EP_DIR_WHITE);
        Position capPosition{ (int8_t)(move.to.row + dir), move.to.col };
        captured = at(capPosition);

    }
    else
    {

        captured = at(move.to);

    }

    MyVector<bool> prevKS = castleKS;
    MyVector<bool> prevQS = castleQS;

    move.execute(*this);

    if (Figure* moved = at(move.to))
    {

        int idx = int(moved->getColor());

        if (moved->getType() == FigureType::KING)
        {

            castleKS[idx] = castleQS[idx] = false;   

        }
        else if (moved->getType() == FigureType::ROOK)
        {

            if (move.from.col == KS_ROOK_FROM_COL) castleKS[idx] = false;
            if (move.from.col == QS_ROOK_FROM_COL) castleQS[idx] = false;

        }

    }

    if (captured && captured->getType() == FigureType::ROOK)
    {

        int idx = int(captured->getColor());

        if (move.to.col == KS_ROOK_FROM_COL) castleKS[idx] = false;
        if (move.to.col == QS_ROOK_FROM_COL) castleQS[idx] = false;

    }

    pushHistory({ move, captured, prevKS, prevQS });

    if (Figure* moved = at(move.to))
        moved->setPosition(move.to);

    if (move.special == SpecialMove::CASTLING_KING_SIDE)
    {

        Position rookSq{ move.to.row, KS_VIA_COL };
        if (Figure* rook = at(rookSq))
            rook->setPosition(rookSq);

    }
    else if (move.special == SpecialMove::CASTLING_QUEEN_SIDE)
    {

        Position rookSq{ move.to.row, QS_VIA_COL };
        if (Figure* rook = at(rookSq))
            rook->setPosition(rookSq);

    }

}

void Board::undoMove(const Move&) // pop last HistoryEntry and roll everything back
{

    HistoryEntry entry = popHistory();
    entry.move.undo(*this);

    castleKS = entry.castleKingSide;   
    castleQS = entry.castleQueenSide;

}

void Board::pushHistory(HistoryEntry entry)
{

    history.push_back(entry);

}

bool Board::canCastleKingSide(MyColor color) const // static tests only-no check/attack verification
{

    int index = (int)(color);
    if (!castleKS[index]) return false;

    int row = (color == MyColor::WHITE ? WHITE_BACK_ROW : BLACK_BACK_ROW);

    Figure* king = at(Position(row, KING_START_COL));
    Figure* rook = at(Position(row, KS_ROOK_FROM_COL));

    if (!king || !rook) return false;
    if (king->getType() != FigureType::KING || rook->getType() != FigureType::ROOK) return false;
    if (at(Position(row, KS_EMPTY_COL1)) || at(Position(row, KS_EMPTY_COL2))) return false;

    return true;

}

bool Board::canCastleQueenSide(MyColor color) const
{

    int index = (int)(color);
    if (!castleQS[index]) return false;

    int row = (color == MyColor::WHITE ? WHITE_BACK_ROW : BLACK_BACK_ROW);

    Figure* king = at(Position(row, KING_START_COL));
    Figure* rook = at(Position(row, KS_ROOK_FROM_COL));

    if (!king || !rook) return false;
    if (king->getType() != FigureType::KING || rook->getType() != FigureType::ROOK) return false;
    if (at(Position(row, QS_EMPTY_COL1)) || at(Position(row, QS_EMPTY_COL2)) || at(Position(row, QS_EMPTY_COL3))) return false;

    return true;

}

bool Board::isUnderAttack(const Position& position, MyColor attacker) const // brute-force scan of every enemy piece
{

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

    Position kingPosition{ INVALID_ROW, INVALID_COL };

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

    if (kingPosition.row < 0) return true;

    return isUnderAttack(kingPosition, oppositeColor(color));

}

bool Board::isEnPassantSquare(const Position& cap, MyColor pawnColor) const
{

    if (history.size() == 0) return false;

    const HistoryEntry& last = history[history.size() - 1];
    const Move& move = last.move;

    if (move.special != SpecialMove::DOUBLE_PAWN) return false;

    int8_t targetRow = (pawnColor == MyColor::WHITE ? EP_ROW_WHITE : EP_ROW_BLACK);

    return (cap.row == targetRow && cap.col == move.to.col);

}

MyVector<Move> Board::generateAllLegalMoves(MyColor side) // loop every friendly piece, keep moves that pass isLegalMove()
{

    MyVector<Move> allMoves;

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

    if (move.special == SpecialMove::EN_PASSANT)
    {

        if (!isEnPassantSquare(move.to, side))
            return false;   

    }

    if (move.special == SpecialMove::CASTLING_KING_SIDE)
    {
        
        if (!canCastleKingSide(side))
            return false;

        Position via{ move.from.row, KS_VIA_COL };
        if (isInCheck(side) ||
            isUnderAttack(via, oppositeColor(side)) ||
            isUnderAttack(move.to, oppositeColor(side)))
            return false;

    }
    else if (move.special == SpecialMove::CASTLING_QUEEN_SIDE)
    {

        if (!canCastleQueenSide(side))
            return false;

        Position via{ move.from.row, QS_VIA_COL };
        if (isInCheck(side) ||
            isUnderAttack(via, oppositeColor(side)) ||
            isUnderAttack(move.to, oppositeColor(side)))
            return false;

    }

    // quick special-case filters, then "make-and-break" test
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

void Board::serialize(Writer& writer) const // 64 bytes to the stream
{

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
        {

            Piece piece = figureToPiece(at({ (int8_t)currentRowIndex,(int8_t)currentColIndex }));
            writer.write(&piece, 1);

        }

    }

}

void Board::deserialize(Reader& reader) // stream to the board
{

    clear();

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
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

bool Board::hasLegalMoves(MyColor side)
{

    return !generateAllLegalMoves(side).empty();

}

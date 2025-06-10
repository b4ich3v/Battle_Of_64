#include "AIPlayer.h"

Move AIPlayer::lastWhite; 

Move AIPlayer::lastBlack;

AIPlayer::AIPlayer(int depth): maxDepth(depth) {}

int AIPlayer::value(FigureType type)
{

    switch (type) 
    {

    case FigureType::PAWN: return VALUE_PAWN;
    case FigureType::KNIGHT: return VALUE_KNIGHT;
    case FigureType::BISHOP: return VALUE_BISHOP;
    case FigureType::ROOK: return VALUE_ROOK;
    case FigureType::QUEEN: return VALUE_QUEEN;
    default: return 0;

    }

}


int AIPlayer::evaluate(const Board& board, MyColor side) const
{

    int score = 0;

    for (int currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
        {

            Figure* currentFigure = nullptr;
            try { currentFigure = board.at({ (int8_t)currentRowIndex,(int8_t)currentColIndex }); }
            catch (...) {}

            if (!currentFigure) continue;

            int currentValue = value(currentFigure->getType());
            score += (currentFigure->getColor() == side) ? currentValue : -currentValue;

        }

    }

    return score;

}

int AIPlayer::search(Board& board, int depth, int alpha, int beta, MyColor side)
{

    if (depth == 0) return evaluate(board, side);

    bool anyMove = false;
    int best = MIN_SCORE;

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
        {

            Figure* currentFigure = nullptr;
            try { currentFigure = board.at({ (int8_t)currentRowIndex,(int8_t)currentColIndex }); }
            catch (...) {}

            if (!currentFigure || currentFigure->getColor() != side) continue;

            auto moves = currentFigure->generateMoves(board, { (int8_t)currentRowIndex,(int8_t)currentColIndex });

            for (size_t i = 0; i < moves.size(); i++)
            {

                if (board.isLegalMove(moves[i], side))
                {
                    anyMove = true;
                    board.applyMove(moves[i]);

                    int score = -search(board, depth - 1, -beta, -alpha, oppositeColor(side));
                    board.undoMove(moves[i]);

                    if (score > best)  best = score;
                    if (score > alpha) alpha = score;
                    if (alpha >= beta) return alpha;

                }

            }

        }

    }

    if (!anyMove) return evaluate(board, side);

    return best;

}

Move AIPlayer::getMove(Board& board, MyColor side)
{

    Move bestMove;
    int bestScore = MIN_SCORE;
    int alpha = MIN_SCORE;
    int beta = BETA_INIT;

    const Move& last = (side == MyColor::WHITE ? lastWhite : lastBlack);

    for (size_t currentRowIndex = 0; currentRowIndex < ROWS_COUNT; currentRowIndex++)
    {

        for (size_t currentColIndex = 0; currentColIndex < COLS_COUNT; currentColIndex++)
        {

            Figure* currentFigure = nullptr;

            try { currentFigure = board.at({ (int8_t)currentRowIndex,(int8_t)currentColIndex }); }
            catch (...) {}

            if (!currentFigure || currentFigure->getColor() != side) continue;
            auto moves = currentFigure->generateMoves(board, { (int8_t)currentRowIndex,(int8_t)currentColIndex });

            for (size_t i = 0; i < moves.size(); i++)
            {

                const Move& currentMove = moves[i];

                if (currentMove.from == last.from && currentMove.to == last.to &&
                    currentMove.promotionType == last.promotionType)
                    continue;

                if (!board.isLegalMove(currentMove, side)) continue;

                board.applyMove(currentMove);
                int score = -search(board, maxDepth - 1, -beta, -alpha, oppositeColor(side));
                board.undoMove(currentMove);

                if (score > bestScore) 
                {

                    bestScore = score;
                    bestMove = currentMove;

                }

                if (score > alpha) alpha = score;

            }

        }

    }
        
    if (side == MyColor::WHITE) lastWhite = bestMove;
    else lastBlack = bestMove;

    return bestMove;

}




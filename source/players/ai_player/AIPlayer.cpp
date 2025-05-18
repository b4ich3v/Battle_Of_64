#include "AIPlayer.h"

AIPlayer::AIPlayer(int depth): maxDepth(depth) {}

int AIPlayer::value(FigureType type)
{

    switch (type)
    {

    case FigureType::PAWN: return 100;
    case FigureType::KNIGHT: return 320;
    case FigureType::BISHOP: return 330;
    case FigureType::ROOK: return 500;
    case FigureType::QUEEN: return 900;
    default: return 0;

    }

}

int AIPlayer::evaluate(const Board& board, MyColor side) const
{

    int score = 0;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
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
    int best = -100000;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
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
    int  bestScore = -100000, alpha = -100000, beta = 100000;

    for (int currentRowIndex = 0; currentRowIndex < 8; currentRowIndex++)
    {

        for (int currentColIndex = 0; currentColIndex < 8; currentColIndex++)
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

                    board.applyMove(moves[i]);

                    int score = -search(board, maxDepth - 1, -beta, -alpha, oppositeColor(side));
                    board.undoMove(moves[i]);

                    if (score > bestScore)
                    {

                        bestScore = score;
                        bestMove = moves[i];

                    }

                    if (score > alpha) alpha = score;

                }

            }

        }

    }

    return bestMove;

}



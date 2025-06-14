#include "AIPlayer.h"

Move AIPlayer::lastWhite; 

Move AIPlayer::lastBlack;

AIPlayer::AIPlayer(Difficulty difficulty): maxDepth((int)(difficulty))
{

    static bool seeded = false;

    if (!seeded)
    {

        // lightweight seed - uses two addresses
        unsigned long long pObj = (unsigned long long)this;
        unsigned long long pVar = (unsigned long long) & seeded;

        // fold 64 to 32
        unsigned a = (unsigned)(pObj ^ (pObj >> OBJ_HIGH_SHIFT));
        unsigned b = (unsigned)(pVar ^ (pVar >> VAR_HIGH_SHIFT));
        unsigned seed = (a << MIX_LEFT_SHIFT) ^ (b >> MIX_RIGHT_SHIFT) ^ XOR_CONST;

        // mini xorshift
        seed ^= seed << XORSHIFT_L;
        seed ^= seed >> XORSHIFT_R;

        srand(seed);
        seeded = true;

    }

    // map difficulty to noise, pool width
    switch (difficulty)
    {

    case Difficulty::Beginner:
    {

        noiseRange = NOISE_BEGINNER;
        marginCp = MARGIN_BEGINNER;
        break;

    }
    case Difficulty::Easy:
    {

        noiseRange = NOISE_EASY;
        marginCp = MARGIN_EASY;
        break;

    }
    case Difficulty::Medium:
    {

        noiseRange = NOISE_MEDIUM;
        marginCp = MARGIN_MEDIUM;
        break;

    }
    case Difficulty::Hard:
    {

        noiseRange = NOISE_HARD;
        marginCp = MARGIN_HARD;
        break;

    }
    default: break;

    }

}

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

    // material loop here
    score += (noiseRange ? (rand() % (2 * noiseRange + 1) - noiseRange) : 0);

    return score;

}

int AIPlayer::search(Board& board, int depth, int alpha, int beta, MyColor side)
{

    if (depth == 0) return evaluate(board, side); // leaf node

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
                    if (alpha >= beta) return alpha; // alpha-beta cut-off

                }

            }

        }

    }

    if (!anyMove) return evaluate(board, side); // checkmate, stalemate

    return best;

}

Move AIPlayer::getMove(Board& board, MyColor side)
{

    Move bestMove;
    int bestScore = MIN_SCORE;
    int alpha = MIN_SCORE;
    int beta = BETA_INIT;
    int candidateCount = 0; // for reservoir sampling

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

                if (score > bestScore) // new leader
                {

                    bestScore = score;
                    bestMove = currentMove;
                    candidateCount = 1;    
           
                }
                else if (bestScore - score <= marginCp) // within pool
                {
                    candidateCount += 1;
                    
                    if (rand() % candidateCount == 0)
                        bestMove = currentMove;

                }

                if (score > alpha) alpha = score;

            }

        }

    }
        
    if (side == MyColor::WHITE) lastWhite = bestMove;
    else lastBlack = bestMove;

    return bestMove; // send to GUI, engine

}




#include "HumanPlayer.h"

Move HumanPlayer::getMove(Board& board, MyColor color) 
{

	// no calculation—simply return what was queued
	return pending; 

}

void HumanPlayer::setPendingMove(const Move& move) 
{

	// cache until engine requests it
	pending = move; 

}


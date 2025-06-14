#include "Player.h"

void Player::setPendingMove(const Move& move) // just cache the move, validation happens elsewhere
{

	pending = move; 

}

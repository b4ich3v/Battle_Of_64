#include "Player.h" 

Player::Player(const MyString& name): 
	name(name) {}

const MyString& Player::getName() const
{

	return name;

}

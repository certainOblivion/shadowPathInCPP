#include "pch.h"
#include "Player.h"

Player::~Player()
{
}

void Player::Update(float dT)
{

}

Player::Player(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc) 
	: IRenderableObject(position, rotation, dimensions, drawFunc)
{}


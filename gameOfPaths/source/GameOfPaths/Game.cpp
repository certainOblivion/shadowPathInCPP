#include "pch.h"
#include "Game.h"

Game::Game(std::function<void*()> getWindow, int gameWidth /*= 800*/, int gameHeight /*= 600*/, const char* gameName /*= "Game"*/): mNavMesh(getWindow, gameWidth, gameHeight)
{

}

Game::~Game()
{
}

void Game::Update(float dT)
{
	mNavMesh.Update(dT);
}


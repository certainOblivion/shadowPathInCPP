#pragma once
#include "SFML\Graphics.hpp"
#include "NavMesh.h"
#include "Library\IUpdatableObject.h"
class Game : Object::IUpdatableObject
{
public:
	Game(std::function<void*()> getWindow, int gameWidth = 800, int gameHeight = 600, const char* gameName = "Game");
	~Game();
	void Update(float dT);
private:
	NavMesh mNavMesh;

};


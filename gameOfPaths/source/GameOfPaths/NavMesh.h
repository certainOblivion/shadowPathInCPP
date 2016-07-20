#pragma once
#include "..\Library\Pathfinding.h"
#include "SFML\Graphics.hpp"
class NavMesh : IUpdatableObject
{
public:
	NavMesh(std::function<void*()> getWindow, int mapWidth, int mapHeight);
	~NavMesh();

	void DrawObstacle(Vector2D position, Vector2D location, float rotation);
	void AddObstacle(Vector2D position, Vector2D location, float rotation);
	void Update(float dT) override;
private:
	PathSystem::Pathfinding mPathfinder;
	std::list<PathObstacle> mObstacles;
	std::list<PathActor> mActors;
	sf::RenderWindow* mWindow;


};


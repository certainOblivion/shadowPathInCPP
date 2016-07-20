#include "pch.h"
#include "NavMesh.h"
#include "SFML\Graphics.hpp"


NavMesh::NavMesh(std::function<void*()> getWindow, int mapWidth, int mapHeight ):mPathfinder(mapWidth, mapHeight,1.f,1.f, mObstacles, mActors), mWindow(nullptr)
{
	mWindow = reinterpret_cast<sf::RenderWindow*>(getWindow());

	AddObstacle(Vector2D(20, 20), Vector2D(200, 100), 20);
}


NavMesh::~NavMesh()
{
}

void NavMesh::DrawObstacle(Vector2D position, Vector2D dimension, float rotation)
{
	sf::RectangleShape shape(sf::Vector2f(dimension.x, dimension.y));
	shape.setPosition(position.x, position.y);
	shape.setRotation(rotation);
	// set the shape color to green
	shape.setFillColor(sf::Color(100, 250, 50));

	mWindow->draw(shape);
}

void NavMesh::AddObstacle(Vector2D position, Vector2D dimension, float rotation)
{
	std::function<void(Vector2D, Vector2D, float)> drawFunc = std::bind(&NavMesh::DrawObstacle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	PathObstacle obstacle(position, 0.f, dimension, &drawFunc);
	mObstacles.emplace_back(obstacle);
	mPathfinder.AddObstacle(obstacle.Position(), obstacle.Dimensions());
}

void NavMesh::Update(float dT)
{
	for (PathObstacle& obstacle: mObstacles)
	{
		obstacle.Draw();
	}
}

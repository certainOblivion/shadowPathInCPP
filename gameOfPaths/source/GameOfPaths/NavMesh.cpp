#include "pch.h"
#include "NavMesh.h"
#include "SFML\Graphics.hpp"
#include "Enemy.h"

using namespace std;

NavMesh::NavMesh(std::function<void*()> getWindow, int mapWidth, int mapHeight ):mPathfinder(mapWidth, mapHeight,1.f,1.f), mWindow(nullptr), mActor(nullptr)
{
	mWindow = reinterpret_cast<sf::RenderWindow*>(getWindow());
	Init();
}

void NavMesh::Init()
{
 	AddObstacle(Vector2D(100, 50), Vector2D(500, 100), 20);
 	AddObstacle(Vector2D(350, 370), Vector2D(400, 100), 20);
 	AddObstacle(Vector2D(1200, 290), Vector2D(600, 100), 20);
 	AddObstacle(Vector2D(820, 450), Vector2D(300, 100), 20);
 	AddObstacle(Vector2D(100, 570), Vector2D(100, 100), 20);
 	AddObstacle(Vector2D(350, 690), Vector2D(400, 100), 20);
 	AddObstacle(Vector2D(600, 1000), Vector2D(500, 100), 20);
 	AddObstacle(Vector2D(1520, 930), Vector2D(300, 100), 20);
 
 
 	std::function<void(Vector2D, Vector2D, float)> drawFunc = std::bind(&NavMesh::DrawPlayer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
 	mActor = std::make_shared<Player>(Vector2D(20,20), 0.f, Vector2D(20,20), &drawFunc);
 	mRenderables.push_front(mActor);
	mUpdatables.push_back(mActor);

	drawFunc = std::bind(&NavMesh::DrawEnemy, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(Vector2D(1120, 1120), 0.f, Vector2D(20, 20), &drawFunc);
	mRenderables.emplace_front(enemy);
	mUpdatables.push_back(enemy);

	shared_ptr<list<Vector2D>> path = make_shared<list<Vector2D>>();
	mPathfinder.GetPath(enemy->Position(), mActor->Position(), *path, mHexInPath, mTestedHex, mTimeToFindPath);
	enemy->SetPath(path);
}
NavMesh::~NavMesh()
{
}

void NavMesh::DrawObstacle(Vector2D position, Vector2D dimension, float rotation)
{
	sf::RectangleShape shape(sf::Vector2f(static_cast<float>(dimension.x), static_cast<float>(dimension.y)));
	shape.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
	shape.setRotation(rotation);
	shape.setFillColor(sf::Color(153, 0, 51));

	mWindow->draw(shape);
}

void NavMesh::DrawPlayer(Vector2D position, Vector2D dimension, float rotation)
{
	sf::CircleShape shape(static_cast<float>(dimension.x));
	shape.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
	shape.setRotation(rotation);
	shape.setFillColor(sf::Color(0,179,255));

	mWindow->draw(shape);
}

void NavMesh::DrawEnemy(Vector2D position, Vector2D dimension, float rotation)
{
	sf::CircleShape shape(static_cast<float>(dimension.x));
	shape.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
	shape.setRotation(rotation);
	shape.setFillColor(sf::Color(255, 162, 122));

	mWindow->draw(shape);
}

void NavMesh::AddObstacle(Vector2D position, Vector2D dimension, float rotation)
{
	std::function<void(Vector2D, Vector2D, float)> drawFunc = std::bind(&NavMesh::DrawObstacle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	std::shared_ptr<PathObstacle> obstacle = std::make_shared<PathObstacle>(position, 0.f, dimension, &drawFunc);
	mRenderables.push_front(obstacle);
	mPathfinder.AddObstacle(obstacle->Position(), obstacle->Dimensions());
}

void NavMesh::Update(float dT)
{
	//logic
	for (std::shared_ptr<Object::IUpdatableObject>& updatable : mUpdatables)
	{
		updatable->Update(dT);
	}
	//draw
	Draw();
}

void NavMesh::Draw() const
{
#if !RELEASE
	std::unordered_set<Grid::Hex> hexSet;
	mPathfinder.GetBlockedHex(hexSet);

	for (const Grid::Hex& hex : hexSet)
	{
		DrawHex(hex, sf::Color::Blue);
	}
#endif

	for (const std::shared_ptr<Object::IRenderableObject>& renderable : mRenderables)
	{
		renderable->Draw();
	}
}

#if !RELEASE
void NavMesh::DrawHex(const Grid::Hex& hex, const sf::Color& color, bool filled /*= false*/) const
{
	std::shared_ptr<std::list<Vector2D>> corners = std::make_shared<std::list<Vector2D>>();
	Grid::Layout::PolygonCorners(mPathfinder.GetLayout(), hex, corners);

	sf::ConvexShape hexShape;
	hexShape.setPointCount(corners->size());
	int index = 0;
	for (Vector2D& corner : *corners)
	{
		hexShape.setPoint(index, sf::Vector2f(static_cast<float>(corner.x), static_cast<float>(corner.y)));
	}
	if (filled)
	{
		hexShape.setFillColor(color);
	}
	else
	{
		hexShape.setOutlineColor(color);
		hexShape.setOutlineThickness(2);
	}

	mWindow->draw(hexShape);
}
#endif

#pragma once
#include "SFML\Graphics.hpp"
#include "Library\Pathfinding.h"
#include "Player.h"
#include "pathobstacle.h"
class NavMesh : public Object::IUpdatableObject
{
public:
	NavMesh(std::function<void*()> getWindow, int mapWidth, int mapHeight);
	void Init();
	~NavMesh();
	void Draw() const;

	void DrawObstacle(Vector2D position, Vector2D location, float rotation);
	void DrawPlayer(Vector2D position, Vector2D dimension, float rotation);
	void DrawEnemy(Vector2D position, Vector2D dimension, float rotation);
	void AddObstacle(Vector2D position, Vector2D location, float rotation);
	void Update(float dT) override;
#if !RELEASE
	void DrawHex(const Grid::Hex& hex, const sf::Color& color, bool filled = false) const;
#endif
private:
	PathSystem::Pathfinding mPathfinder;
	std::shared_ptr<Player> mActor;
	sf::RenderWindow* mWindow;
	std::list<std::shared_ptr<Object::IRenderableObject>> mRenderables;
	std::list<std::shared_ptr<Object::IUpdatableObject>> mUpdatables;

#if !RELEASE
	std::list<Grid::Hex> mHexInPath;
	std::list<Grid::Hex> mTestedHex;
	long mTimeToFindPath;
#endif
};


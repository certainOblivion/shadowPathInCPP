#pragma once
#include "SFML\Graphics.hpp"
#include "Library\Pathfinding.h"
#include "Player.h"
#include "pathobstacle.h"
#include "Enemy.h"
#include "Library\VisibilityComputer.h"
class Game : public Object::IUpdatableObject
{
public:
    Game(std::function<void*()> getWindow, int mapWidth, int mapHeight);
    void Init();
    ~Game();
    void Draw() const;

    void DrawHUD() const;
    void DrawObstacle(Vector2D position, Vector2D location, float rotation);
    void DrawPlayer(Vector2D position, Vector2D dimension, float rotation);
    void DrawEnemy(Vector2D position, Vector2D dimension, float rotation);
    void AddObstacle(Vector2D position, Vector2D location, float rotation);
    void Update(float dT) override;
#if !RELEASE
    void DrawHex(const Grid::Hex& hex, const sf::Color& color, bool filled = false) const;
    void DrawHex(const Vector2D& hexPos, const sf::Color& color, bool filled = false) const;
#endif
private:
    PathSystem::Pathfinding mPathfinder;
    Visibility::VisibilityComputer mVisibility;
    std::shared_ptr<Player> mPlayer;
    std::shared_ptr<Enemy> mEnemy;
    sf::RenderWindow* mWindow;
    std::list<std::shared_ptr<Object::IRenderableObject>> mRenderables;
    std::list<std::shared_ptr<Object::IUpdatableObject>> mUpdatables;
    std::vector<Vector2D> mVisibilityPoints;
    float mFPSCounter;
#if !RELEASE
    std::list<Grid::Hex> mHexInPath;
    std::unordered_set<Grid::Hex> mTestedHex;
    std::shared_ptr<std::list<Vector2D>> mPath;
    long mTimeToFindPath;
#endif
};


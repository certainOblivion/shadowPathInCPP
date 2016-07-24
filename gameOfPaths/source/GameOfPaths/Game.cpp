#include "pch.h"
#include "Game.h"
#include "SFML\Graphics.hpp"
#include "Enemy.h"
#include "SFML\Graphics\Color.hpp"
#include "Helper.h"
#include "SFML\Graphics.hpp"
#include <iostream>

using namespace std;
using namespace Helper;
using namespace sf;
using namespace Grid;

Game::Game(std::function<void*()> getWindow, int mapWidth, int mapHeight) 
    :mPathfinder(mapWidth, mapHeight, 20.f), mWindow(nullptr), mPlayer(nullptr), mVisibility(Vector2D(), 5000.f)
#if !RELEASE
, mPath(std::make_shared<std::list<Vector2D>>())
#endif
{
    mWindow = reinterpret_cast<sf::RenderWindow*>(getWindow());
    Init();
}

void Game::Init()
{
    AddObstacle(Vector2D(-400, 500), Vector2D(500, 100), 20);
    AddObstacle(Vector2D(350, 340), Vector2D(400, 100), 20);
    AddObstacle(Vector2D(-600, 290), Vector2D(600, 100), 20);
    AddObstacle(Vector2D(420, -450), Vector2D(300, 100), 20);
    AddObstacle(Vector2D(100, 0), Vector2D(100, 500), 20);
    AddObstacle(Vector2D(350, 120), Vector2D(400, 100), 20);
    AddObstacle(Vector2D(-350, -120), Vector2D(500, 100), 20);
    AddObstacle(Vector2D(-700, -480), Vector2D(300, 100), 20);


    std::function<void(Vector2D, Vector2D, float)> drawFunc = std::bind(&Game::DrawPlayer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    mPlayer = std::make_shared<Player>(Vector2D(-20, 20), 0.f, Vector2D(10, 10), &drawFunc);
    mRenderables.push_front(mPlayer);
    mUpdatables.push_back(mPlayer);

    drawFunc = std::bind(&Game::DrawEnemy, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    mEnemy = std::make_shared<Enemy>(Vector2D(-390, 140), 0.f, Vector2D(10, 10), &drawFunc);
    mRenderables.emplace_front(mEnemy);
    mUpdatables.push_back(mEnemy);
}
Game::~Game()
{
}

void Game::DrawObstacle(Vector2D position, Vector2D dimension, float rotation)
{
    sf::RectangleShape shape(sf::Vector2f(static_cast<float>(dimension.x), static_cast<float>(dimension.y)));
    Vector2f screenPosition = WorldToScreenPoint(position);
    shape.setPosition(screenPosition);
    shape.setRotation(-rotation);
    shape.setOrigin(Vector2f(static_cast<float>(dimension.x / 2), static_cast<float>(dimension.y / 2)));
    shape.setFillColor(sf::Color(153, 0, 51));
    mWindow->draw(shape);
}

void Game::DrawPlayer(Vector2D position, Vector2D dimension, float rotation)
{
    sf::CircleShape shape(static_cast<float>(dimension.x));
    Vector2f screenPosition = WorldToScreenPoint(position);
    shape.setPosition(screenPosition);
    shape.setOrigin(Vector2f(static_cast<float>(9 * dimension.x / 10), static_cast<float>(9 * dimension.y / 10)));
    shape.setRotation(rotation);
    shape.setFillColor(sf::Color(0, 179, 255));

#if !RELEASE
    DrawHex(Grid::FractionalHex::HexRound(Grid::Layout::PixelToHex(mPathfinder.GetLayout(), mPlayer->Position())), sf::Color::Blue, false);
#endif

    mWindow->draw(shape);
}

void Game::DrawEnemy(Vector2D position, Vector2D dimension, float rotation)
{
    sf::CircleShape shape(static_cast<float>(dimension.x));
    Vector2f screenPosition = WorldToScreenPoint(position);
    shape.setPosition(screenPosition);
    shape.setOrigin(Vector2f(static_cast<float>(9 * dimension.x / 10), static_cast<float>(9 * dimension.y / 10)));
    shape.setRotation(rotation);
    shape.setFillColor(sf::Color(255, 162, 122));
#if !RELEASE
    DrawHex(Grid::FractionalHex::HexRound(Grid::Layout::PixelToHex(mPathfinder.GetLayout(), mEnemy->Position())), sf::Color::Red, false);
#endif // !RELEASE

    mWindow->draw(shape);

}

void Game::AddObstacle(Vector2D position, Vector2D dimension, float rotation)
{
    std::function<void(Vector2D, Vector2D, float)> drawFunc = std::bind(&Game::DrawObstacle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    std::shared_ptr<PathObstacle> obstacle = std::make_shared<PathObstacle>(position, rotation, dimension, &drawFunc);
    mRenderables.push_front(obstacle);
    mPathfinder.AddObstacle(obstacle->Position(), obstacle->Dimensions(), rotation * Helper::MathHelper::Deg2Rad);
}

void Game::Update(float dT)
{
    float fps = 1 / dT;
    cout << fps << "\n";
    //handle input
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        Vector2f mousePos;
        mousePos.x = static_cast<float>(sf::Mouse::getPosition(*mWindow).x);

        mousePos.y = static_cast<float>(sf::Mouse::getPosition(*mWindow).y);
        Vector2D worldPoint = ScreenToWorldPoint(mousePos);
        mPlayer->SetPosition(worldPoint);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        Vector2f mousePos;
        mousePos.x = static_cast<float>(sf::Mouse::getPosition(*mWindow).x);

        mousePos.y = static_cast<float>(sf::Mouse::getPosition(*mWindow).y);
        Vector2D worldPoint = ScreenToWorldPoint(mousePos);
        mEnemy->SetPosition(worldPoint);
#if !RELEASE
        mPath->clear();
        mPathfinder.GetPath(mEnemy->Position(), mPlayer->Position(), *mPath, mHexInPath, mTestedHex, mTimeToFindPath);
        cout << "time to find path: " << mTimeToFindPath << "\n";
        mEnemy->SetPath(mPath);
#else
        shared_ptr<list<Vector2D>> path = make_shared<list<Vector2D>>();
        mPathfinder.GetPath(mEnemy->Position(), mPlayer->Position(), *path);
        mEnemy->SetPath(path);
#endif // !RELEASE

    }

    for (std::shared_ptr<Object::IUpdatableObject>& updatable : mUpdatables)
    {
        updatable->Update(dT);
    }

    //calculate shadows
    mVisibility.SetOrigin(mPlayer->Position());

    //add all obstacles' current position and dimensions except the player
    for (const std::shared_ptr<Object::IRenderableObject>& renderable : mRenderables)
    {
        if (renderable != mPlayer)
        {
            mVisibility.AddRectangleOccluder(renderable->Position(), renderable->Dimensions(), renderable->RotationInRadians());
        }
    }
    mVisibilityPoints.clear();
    mVisibility.GetVisibility(mVisibilityPoints);
    if (mVisibilityPoints.back() != mVisibilityPoints.front())
    {
        mVisibilityPoints.emplace_back(mVisibilityPoints.front());
    }
    mVisibility.ClearOccluders();
}

void Game::Draw() const
{
    for (int i = 1; i < mVisibilityPoints.size(); i++)
    {
        //populate the 4 sides of the trapezoid clockwise
        const Vector2D& p1 = mVisibilityPoints[i - 1];
        const Vector2D& p2 = mVisibilityPoints[i];

        const Vector2D& origin = mVisibility.Origin();
        //calculate p3 (projected from p2) and p4 (projection from p1)
        
        Vector2D p3 = p2 + ((p2 - origin) / (p2 - origin).Magnitude() * 1000000.f);
        Vector2D p4 = p1 + ((p1 - origin) / (p1 - origin).Magnitude() * 1000000.f);

        sf::ConvexShape shadowShape;
        shadowShape.setPointCount(4);
        shadowShape.setPoint(0, WorldToScreenPoint(p1));
        shadowShape.setPoint(1, WorldToScreenPoint(p2));
        shadowShape.setPoint(2, WorldToScreenPoint(p3));
        shadowShape.setPoint(3, WorldToScreenPoint(p4));

        shadowShape.setFillColor(Color(100,100,100));

        mWindow->draw(shadowShape);
    }

    for (const std::shared_ptr<Object::IRenderableObject>& renderable : mRenderables)
    {
        renderable->Draw();
    }

#if !RELEASE
    std::unordered_set<Grid::Hex> hexSet;
    mPathfinder.GetBlockedHex(hexSet);

    for (const Grid::Hex& hex : hexSet)
    {
        DrawHex(hex, sf::Color::Blue);
    }

    for (const Grid::Hex& hex : mTestedHex)
    {
        DrawHex(hex, sf::Color::Black);
    }

    for (const Grid::Hex& hex : mHexInPath)
    {
        DrawHex(hex, sf::Color::White);
    }

    for (const Vector2D& pos : *mPath)
    {
        DrawHex(pos, sf::Color::Green);
    }

#endif

    for (const Vector2D& point : mVisibilityPoints )
    {
        Vertex v = WorldToScreenPoint(point);
        v.color = Color::Cyan;
        mWindow->draw(&v, 1, sf::Points);
    }
}

#if !RELEASE
void Game::DrawHex(const Grid::Hex& hex, const sf::Color& color, bool filled /*= false*/) const
{
    std::shared_ptr<std::list<Vector2D>> corners = std::make_shared<std::list<Vector2D>>();
    Grid::Layout::PolygonCorners(mPathfinder.GetLayout(), hex, corners);

    sf::ConvexShape hexShape;
    hexShape.setPointCount(corners->size());
    int index = 0;
    for (Vector2D& corner : *corners)
    {
        Vector2f screenPoint = WorldToScreenPoint(corner);
        hexShape.setPoint(index++, screenPoint);
    }
    if (filled)
    {
        hexShape.setFillColor(color);
    }
    else
    {
        hexShape.setFillColor(sf::Color::Transparent);
        hexShape.setOutlineColor(color);
        hexShape.setOutlineThickness(2);
    }

    mWindow->draw(hexShape);
}

void Game::DrawHex(const Vector2D& hexPos, const sf::Color& color, bool filled /*= false*/) const
{
    Hex& hexAtPos = FractionalHex::HexRound(Layout::PixelToHex(mPathfinder.GetLayout(), hexPos));
    DrawHex(hexAtPos, color, filled);
}

#endif

#include "pch.h"
#include "Game.h"
#include "Enemy.h"
#include "SFML\Graphics.hpp"
#include "assetloader.h"
#include <iostream>
#include "GameData.h"
#include "SFML\Graphics.hpp"
#include "Library\IObject.h"

using namespace std;
using namespace Helper;
using namespace sf;
using namespace Grid;
using namespace Object;

Game::Game(std::function<void*()> getWindow)
    : mWindow(nullptr)
#if !RELEASE
    , mPath(std::make_shared<std::list<Vector2D>>())
#endif
{
    mWindow = reinterpret_cast<sf::RenderWindow*>(getWindow());
    Init();
}

void Game::Init()
{
    GameData& gameInfo = GameData::GetPtr();
    gameInfo.Init(mWindow);

    mMap = std::make_shared<Map>();
    mRenderables.push_back(mMap);

    mEnemyManager = std::make_shared<EnemyManager>(2, *mMap);
    mUpdatables.push_back(mEnemyManager);
    mRenderables.push_back(mEnemyManager);

    Object::Transform playerSpawnTransform = mMap->GetPlayerSpawnTransform();
    AddPlayer(playerSpawnTransform);
}

void Game::Update(float dT)
{
    //fps string will change every frame
    mFPSCounter = 1 / dT;

    shared_ptr<Player> player = GameData::GetPtr().GetPlayer();
    //handle input
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        Vector2f mousePos;
        mousePos.x = static_cast<float>(sf::Mouse::getPosition(*mWindow).x);

        mousePos.y = static_cast<float>(sf::Mouse::getPosition(*mWindow).y);
        Vector2D worldPoint = GameData::ScreenToWorldPoint(mousePos);
        player->SetPosition(worldPoint);
    }

    for (std::shared_ptr<Object::IUpdatableObject>& updatable : mUpdatables)
    {
        updatable->Update(dT);
    }

    //calculate shadows
    shared_ptr<Visibility::VisibilityComputer> visibility = GameData::GetPtr().GetVisibility();

    visibility->SetOrigin(player->Position());

    //add all obstacles' current position and dimensions except the player
    for (const std::shared_ptr<PathObstacle>& transform : mMap->GetObstacles())
    {
        visibility->AddRectangleOccluder(transform->Position(), transform->Dimensions(), transform->RotationInRadians());
    }
    mVisibilityPoints.clear();
    visibility->GetVisibility(mVisibilityPoints);
    if (mVisibilityPoints.back() != mVisibilityPoints.front())
    {
        mVisibilityPoints.emplace_back(mVisibilityPoints.front());
    }
    visibility->ClearOccluders();
}

void Game::Draw()
{
    //draw visible area
    for (size_t i = 1; i < mVisibilityPoints.size(); i++)
    {
        //populate the 4 sides of the trapezoid clockwise
        const Vector2D& p1 = mVisibilityPoints[i - 1];
        const Vector2D& p2 = mVisibilityPoints[i];

        const Vector2D& origin = GameData::GetPtr().GetVisibility()->Origin();

        sf::ConvexShape shadowShape;
        shadowShape.setPointCount(3);
        shadowShape.setPoint(0, GameData::WorldToScreenPoint(p1));
        shadowShape.setPoint(1, GameData::WorldToScreenPoint(p2));
        shadowShape.setPoint(2, GameData::WorldToScreenPoint(origin));

        shadowShape.setFillColor(Color(136, 136, 136));
        mWindow->draw(shadowShape);
    }

    //call all the renderables' draw
    for (const std::shared_ptr<Object::IRenderableObject>& renderable : mRenderables)
    {
        renderable->Draw();
    }

    DrawHUD();
}

void Game::DrawHUD() const
{

#if !RELEASE
    //TODO: draw buttons to toggle debug draw
    
    // debug draw
     GameData& gameInfo = GameData::GetPtr();
     for (const Vector2D& hex : mMap->GetAllWalkablePoints())
     {
         gameInfo.DrawHex(hex, sf::Color::White);
     }

     std::unordered_set<Grid::Hex> hexSet;
     gameInfo.GetPathFinder()->GetBlockedHexList(hexSet);

     for (const Grid::Hex& hex : hexSet)
     {
         gameInfo.DrawHex(hex, sf::Color::Red);
     }

     for (const Grid::Hex& hex : mTestedHex)
     {
         gameInfo.DrawHex(hex, sf::Color::Blue);
     }

     for (const Grid::Hex& hex : mHexInPath)
     {
         gameInfo.DrawHex(hex, sf::Color::Cyan);
     }

     for (const Vector2D& pos : *mPath)
     {
         gameInfo.DrawHex(pos, sf::Color::Green);
     }

#endif
    //draw fps text
    Text fpsText;
    Font fpsFont;
    unsigned int fontSize = 0;
    const char* fontData = AssetLoader::GetAsset("resources\\Now-Light.otf", fontSize);

    Vector2D screenDimensions = GameData::GetPtr().GetScreenDimensions();
    fpsFont.loadFromMemory(fontData, fontSize);
    fpsText.setPosition(static_cast<float>(screenDimensions.x) * .95f, static_cast<float>(screenDimensions.y) * .01f);
    fpsText.setFont(fpsFont);
    char fpsString[8]; // 4 digits before decimal + 1 decimal character + 2 digits after decimal + 1 null
    sprintf_s(fpsString, "%.2f", mFPSCounter);
    fpsText.setString(fpsString);

    Color fpsColor = mFPSCounter > 40 ? Color::Green : (mFPSCounter > 20 ? Color::Yellow : Color::Red);
    fpsText.setColor(fpsColor);
    mWindow->draw(fpsText);
}


void Game::AddPlayer(const Object::Transform& playerSpawnTransform)
{ 
    std::shared_ptr<Player> player = GameData::GetPtr().CreatePlayer(playerSpawnTransform.Position(), playerSpawnTransform.Dimensions(), playerSpawnTransform.RotationInDegrees());
    mRenderables.push_front(player);
    mUpdatables.push_back(player);
}

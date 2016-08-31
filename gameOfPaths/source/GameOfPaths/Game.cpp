#include "pch.h"
#include "Game.h"
#include "Enemy.h"
#include "SFML\Graphics.hpp"
#include "assetloader.h"
#include <iostream>
#include "GameHelper.h"
#include "SFML\Graphics.hpp"
#include "Library\IObject.h"
#include "PathFinderManager.h"

using namespace std;
using namespace Helper;
using namespace sf;
using namespace Grid;
using namespace Object;

Game::Game(std::function<void*()> getWindow)
    : mWindow(nullptr)
#if !RELEASE
    , mEnableDrawBlockedHex(true)
#endif
{
    mWindow = reinterpret_cast<sf::RenderWindow*>(getWindow());
    Init();
}

void Game::Init()
{
    GameHelper& gameInfo = GameHelper::GetInstance();
    gameInfo.Init(mWindow);

    PathFinderManager::Start();

    mMap = std::make_shared<Map>();
    mRenderables.push_back(mMap);

#if !RELEASE
    PathFinderManager::GetBlockedHexArray(mBlockedHex);
#endif

    mEnemyManager = std::make_shared<EnemyManager>(10, *mMap);
    mUpdatables.push_back(mEnemyManager);
    mRenderables.push_back(mEnemyManager);

    Object::Transform playerSpawnTransform = mMap->GetPlayerSpawnTransform();
    AddPlayer(playerSpawnTransform);
}

void Game::Update(float dT)
{
    //fps string will change every frame
    mFPSCounter = 1 / dT;
    HandleInput();

    for (std::shared_ptr<Object::IUpdatableObject>& updatable : mUpdatables)
    {
        updatable->Update(dT);
    }

    CalculateVisibility();
}

void Game::Draw()
{
    //draw visible area
    for (size_t i = 1; i < mVisibilityPoints.size(); i++)
    {
        //populate the 4 sides of the trapezoid clockwise
        const Vector2D& p1 = mVisibilityPoints[i - 1];
        const Vector2D& p2 = mVisibilityPoints[i];

        const Vector2D& origin = GameHelper::GetInstance().GetVisibility()->Origin();

        sf::ConvexShape shadowShape;
        shadowShape.setPointCount(3);
        shadowShape.setPoint(0, GameHelper::WorldToScreenPoint(p1));
        shadowShape.setPoint(1, GameHelper::WorldToScreenPoint(p2));
        shadowShape.setPoint(2, GameHelper::WorldToScreenPoint(origin));

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
    //draw fps text
    Text fpsText;
    Font font;
    unsigned int fontSize = 0;
    const char* fontData = AssetLoader::GetAsset("resources\\Now-Light.otf", fontSize);

    Vector2D screenDimensions = GameHelper::GetInstance().GetScreenDimensions();
    font.loadFromMemory(fontData, fontSize);
    fpsText.setPosition(static_cast<float>(screenDimensions.x) * .95f, static_cast<float>(screenDimensions.y) * .01f);
    fpsText.setFont(font);
    char fpsString[8]; // 4 digits before decimal + 1 decimal character + 2 digits after decimal + 1 null
    sprintf_s(fpsString, "%.2f", mFPSCounter);
    fpsText.setString(fpsString);

    Color fpsColor = mFPSCounter > 40 ? Color::Green : (mFPSCounter > 20 ? Color::Yellow : Color::Red);
    fpsText.setFillColor(fpsColor);
    mWindow->draw(fpsText);

#if !RELEASE
    if (mEnableDrawBlockedHex)
    {
        for (const Grid::Hex& h : mBlockedHex)
        {
            GameHelper::DrawHex(h, sf::Color::Red);
        }
    }

    const char* debugString = "Press 1 to disable drawing blocked hex";
    const int characterSize = 20;
    Text debugText;
    debugText.setFont(font);
    debugText.setCharacterSize(characterSize);
    debugText.setString(debugString);
    debugText.setPosition(static_cast<float>(screenDimensions.x) * .01f, static_cast<float>(screenDimensions.y) * .95f);
    debugText.setFillColor(Color::White);

    mWindow->draw(debugText);
#endif
}


void Game::AddPlayer(const Object::Transform& playerSpawnTransform)
{ 
    std::shared_ptr<Player> player = GameHelper::GetInstance().CreatePlayer(playerSpawnTransform.Position(), playerSpawnTransform.Dimensions(), playerSpawnTransform.RotationInDegrees());
    mRenderables.push_front(player);
    mUpdatables.push_back(player);
}

Game::~Game()
{
    PathFinderManager::Stop();
}

void Game::HandleInput()
{
#if !RELEASE
    //handle debug input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
    {
        if (mKeysPressed.count(sf::Keyboard::Num1) == 0)
        {
            mKeysPressed.insert(sf::Keyboard::Num1);
            mEnableDrawBlockedHex = !mEnableDrawBlockedHex;
        }
    }
#endif

    //enable keys strikes for keys that have been released
    auto it = mKeysPressed.begin();

    while (it != mKeysPressed.end())
    {
        if (!sf::Keyboard::isKeyPressed(*it))
        {
            it = mKeysPressed.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::CalculateVisibility()
{
    //calculate shadows from the Player's perspective
    shared_ptr<Player> player = GameHelper::GetInstance().GetPlayer();
    shared_ptr<Visibility::VisibilityComputer> visibility = GameHelper::GetInstance().GetVisibility();

    visibility->SetOrigin(player->Position());

    //add all obstacles' current position and dimensions except the player
    for (const std::shared_ptr<PathObstacle>& transform : mMap->GetObstacles())
    {
        visibility->AddRectangleOccluder(transform->Position(), transform->Dimensions(), transform->RotationInRadians());
    }
    mVisibilityPoints.clear();
    visibility->GetVisibility(mVisibilityPoints);
    visibility->ClearOccluders();
}

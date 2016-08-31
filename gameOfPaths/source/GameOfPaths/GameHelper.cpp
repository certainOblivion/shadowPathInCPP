#include "pch.h"
#include "GameHelper.h"
#include "SFML\Graphics.hpp"
#include "Library\Pathfinding.h"
#include "Library\VisibilityComputer.h"
#include <functional>
#include "pathobstacle.h"
#include "Library\MathHelper.h"
#include "PathFinderManager.h"
#include "AssetLoader.h"
#include "Box2D.h"

using namespace std;
using namespace Grid;
using namespace sf;

sf::RenderWindow* GameHelper::mWindow = nullptr;

//////////////////////////////////////////////////////////////////////////
GameHelper::GameHelper() 
{

}

//////////////////////////////////////////////////////////////////////////
sf::Vector2f GameHelper::WorldToScreenPoint(const Vector2D& worldPoint)
{
    double x = worldPoint.x;
    double y = worldPoint.y;
    Vector2D screenDimensions = GetInstance().GetScreenDimensions();
    x += (screenDimensions.x / 2);
    y = screenDimensions.y - y - (screenDimensions.y / 2);

    return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}

//////////////////////////////////////////////////////////////////////////
Vector2D GameHelper::ScreenToWorldPoint(const sf::Vector2f& screenPoint)
{
    float x = screenPoint.x;
    float y = screenPoint.y;
    Vector2D screenDimensions = GetInstance().GetScreenDimensions();
    x -= (static_cast<float>(screenDimensions.x) / 2);
    y = static_cast<float>(screenDimensions.y) - y - (static_cast<float>(screenDimensions.y) / 2);

    return Vector2D(static_cast<double>(x), static_cast<double>(y));
}

float GameHelper::WorldToScreenRotation(float rotation)
{
    return -rotation - 90;
}

//////////////////////////////////////////////////////////////////////////
std::wstring GameHelper::ExecutableDirectory()
{
    WCHAR buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    PathRemoveFileSpec(buffer);

    return std::wstring(buffer);
}

//////////////////////////////////////////////////////////////////////////
sf::RenderWindow* GameHelper::GetWindow()
{
    return mWindow;
}

//////////////////////////////////////////////////////////////////////////
Vector2D GameHelper::GetScreenDimensions() const
{
    return mScreenDimensions;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<Player> GameHelper::CreatePlayer(const Vector2D& spawnPosition, const Vector2D& playerDimensions, float playerRotation)
{
    mPlayer = std::make_shared<Player>(spawnPosition, playerRotation, playerDimensions);
    return mPlayer;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<Player> GameHelper::GetPlayer() const
{
    return mPlayer;
}

//////////////////////////////////////////////////////////////////////////
void GameHelper::Init(sf::RenderWindow * window)
{
    mWindow = window;
    mScreenDimensions = Vector2D(static_cast<float>(mWindow->getView().getSize().x), static_cast<float>(mWindow->getView().getSize().y));
    mVisibility = std::make_shared<Visibility::VisibilityComputer>(Vector2D(), 5000.f);

    PathFinderManager::GetInstance().Init(static_cast<int>(mScreenDimensions.x), static_cast<int>(mScreenDimensions.y), 15.f);

    b2Vec2 gravity(0.0f, 0.0f);
    b2World world(gravity);
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<Visibility::VisibilityComputer> GameHelper::GetVisibility()
{
    return mVisibility;
}

//////////////////////////////////////////////////////////////////////////
GameHelper& GameHelper::GameHelper::GetInstance()
{
    static GameHelper mPtr;
    return mPtr;
}

//////////////////////////////////////////////////////////////////////////
std::shared_ptr<PathObstacle> GameHelper::AddObstacle(Vector2D position, Vector2D dimension, float rotation, bool isVisible /*= true*/, bool isHollow /*= false*/)
{
    std::shared_ptr<PathObstacle> obstacle = std::make_shared<PathObstacle>(position, rotation, dimension, isVisible, isHollow);

    PathFinderManager::AddObstacle(position, dimension, rotation, isVisible, isHollow);

    return obstacle;
}

//////////////////////////////////////////////////////////////////////////
#if !RELEASE
void GameHelper::DrawHex(const Grid::Hex& hex, const sf::Color& color, bool filled /*= false*/) 
{
    std::shared_ptr<std::list<Vector2D>> corners = std::make_shared<std::list<Vector2D>>();
    Grid::Layout::PolygonCorners(PathFinderManager::GetHexGridLayout(), hex, corners);

    sf::ConvexShape hexShape;
    hexShape.setPointCount(corners->size());
    int index = 0;
    for (Vector2D& corner : *corners)
    {
        Vector2f screenPoint = GameHelper::WorldToScreenPoint(corner);
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

//     Text hexText;
//     Font hexFont;
//     unsigned int fontSize = 0;
//     const char* fontData = AssetLoader::GetAsset("resources\\Now-Light.otf", fontSize);
// 
//     Vector2D hexPosition = Layout::HexToPixel(PathFinderManager::GetHexGridLayout(), hex);
//     hexFont.loadFromMemory(fontData, fontSize);
//     hexText.setPosition(WorldToScreenPoint(hexPosition - Vector2D(15,-10)));
//     hexText.setFont(hexFont);
//     hexText.setCharacterSize(10);
//     char hexString[15];
//     sprintf_s(hexString, "%d,%d\n  %d", hex.q,hex.r,hex.s);
//     hexText.setString(hexString);
//     hexText.setColor(sf::Color::White);
//     mWindow->draw(hexText);
}

//////////////////////////////////////////////////////////////////////////
void GameHelper::DrawHex(const Vector2D& hexPos, const sf::Color& color, bool filled /*= false*/) 
{
    Hex hexAtPos = FractionalHex::HexRound(Layout::PixelToHex(PathFinderManager::GetHexGridLayout(), hexPos));
    DrawHex(hexAtPos, color, filled);
}

#endif
#include "pch.h"
#include "GameData.h"
#include "SFML\Graphics.hpp"
#include "Library\Pathfinding.h"
#include "Library\VisibilityComputer.h"
#include <functional>
#include "pathobstacle.h"
#include "Library\MathHelper.h"

using namespace std;
using namespace Grid;
using namespace sf;

sf::RenderWindow* GameData::mWindow = nullptr;

//////////////////////////////////////////////////////////////////////////
GameData::GameData() 
{

}

//////////////////////////////////////////////////////////////////////////
sf::Vector2f GameData::WorldToScreenPoint(const Vector2D& worldPoint)
{
    double x = worldPoint.x;
    double y = worldPoint.y;
    Vector2D screenDimensions = GetPtr().GetScreenDimensions();
    x += (screenDimensions.x / 2);
    y = screenDimensions.y - y - (screenDimensions.y / 2);

    return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}

//////////////////////////////////////////////////////////////////////////
Vector2D GameData::ScreenToWorldPoint(const sf::Vector2f& screenPoint)
{
    float x = screenPoint.x;
    float y = screenPoint.y;
    Vector2D screenDimensions = GetPtr().GetScreenDimensions();
    x -= (static_cast<float>(screenDimensions.x) / 2);
    y = static_cast<float>(screenDimensions.y) - y - (static_cast<float>(screenDimensions.y) / 2);

    return Vector2D(static_cast<double>(x), static_cast<double>(y));
}

float GameData::WorldToScreenRotation(float rotation)
{
    return -rotation - 90;
}

//////////////////////////////////////////////////////////////////////////
std::wstring GameData::ExecutableDirectory()
{
    WCHAR buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    PathRemoveFileSpec(buffer);

    return std::wstring(buffer);
}

sf::RenderWindow* GameData::GetWindow()
{
    return mWindow;
}

Vector2D GameData::GetScreenDimensions() const
{
    return mScreenDimensions;
}

std::shared_ptr<Player> GameData::CreatePlayer(const Vector2D& spawnPosition, const Vector2D& playerDimensions, float playerRotation)
{
    mPlayer = std::make_shared<Player>(spawnPosition, playerRotation, playerDimensions);
    return mPlayer;
}

std::shared_ptr<Player> GameData::GetPlayer() const
{
    return mPlayer;
}

void GameData::Init(sf::RenderWindow * window)
{
    mWindow = window;
    mScreenDimensions = Vector2D(static_cast<float>(mWindow->getSize().x), static_cast<float>(mWindow->getSize().y));
    mPathfinder = std::make_shared<PathSystem::Pathfinding>(static_cast<int>(mScreenDimensions.x), static_cast<int>(mScreenDimensions.y), 20.f);
    mVisibility = std::make_shared<Visibility::VisibilityComputer>(Vector2D(), 5000.f);
}

std::shared_ptr<PathSystem::Pathfinding> GameData::GetPathFinder()
{
    return mPathfinder;
}

std::shared_ptr<Visibility::VisibilityComputer> GameData::GetVisibility()
{
    return mVisibility;
}

GameData& GameData::GameData::GetPtr()
{
    static GameData mPtr;

    return mPtr;
}

std::shared_ptr<PathObstacle> GameData::AddObstacle(Vector2D position, Vector2D dimension, float rotation, bool isVisible /*= true*/, bool isHollow /*= false*/)
{
    std::shared_ptr<PathObstacle> obstacle = std::make_shared<PathObstacle>(position, rotation, dimension, isVisible, isHollow);
    if (!isHollow)
    {
        mPathfinder->AddObstacle(obstacle->Position(), obstacle->Dimensions(), rotation * Helper::MathHelper::Deg2Rad);
    }
    else
    {
        vector<Vector2D> corners;
        Helper::MathHelper::GetRectangleCorners(position, dimension, rotation * Helper::MathHelper::Deg2Rad, corners);

        mPathfinder->AddBlockedLine(corners[0], corners[1]);
        mPathfinder->AddBlockedLine(corners[1], corners[2]);
        mPathfinder->AddBlockedLine(corners[2], corners[3]);
        mPathfinder->AddBlockedLine(corners[3], corners[0]);
    }

    return obstacle;
}

#if !RELEASE
void GameData::DrawHex(const Grid::Hex& hex, const sf::Color& color, bool filled /*= false*/) 
{
    std::shared_ptr<std::list<Vector2D>> corners = std::make_shared<std::list<Vector2D>>();
    Grid::Layout::PolygonCorners(GetPtr(). mPathfinder->GetLayout(), hex, corners);

    sf::ConvexShape hexShape;
    hexShape.setPointCount(corners->size());
    int index = 0;
    for (Vector2D& corner : *corners)
    {
        Vector2f screenPoint = GameData::WorldToScreenPoint(corner);
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

void GameData::DrawHex(const Vector2D& hexPos, const sf::Color& color, bool filled /*= false*/) 
{
    Hex& hexAtPos = FractionalHex::HexRound(Layout::PixelToHex(GetPtr().mPathfinder->GetLayout(), hexPos));
    DrawHex(hexAtPos, color, filled);
}

#endif
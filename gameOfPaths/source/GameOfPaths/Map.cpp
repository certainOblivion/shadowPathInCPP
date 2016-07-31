#include "pch.h"
#include "Map.h"
#include "Library\XmlParseMaster.h"
#include "LevelXmlParseHelper.h"
#include "LevelSharedData.h"
#include "pathobstacle.h"

using namespace Library;
using namespace std;
Map::Map()
{
    Init();
}


Map::~Map()
{
}

void Map::Init()
{
    std::string fileName = "resources\\level1.xml";

    std::shared_ptr<LevelSharedData> lvlSharedData = std::make_shared<LevelSharedData>();
    XmlParseMaster masterParser(lvlSharedData);
    LevelXmlParseHelper levelHelper;

    masterParser.AddHelper(levelHelper);
    masterParser.ParseFromFile(fileName);

    for (size_t i = 0; i < lvlSharedData->GetObstacleCount(); i++)
    {
        shared_ptr<PathObstacle> newObstacle = GameData::GetPtr().AddObstacle(lvlSharedData->GetObstaclePosition(i), lvlSharedData->GetObstacleDimensions(i), lvlSharedData->GetObstacleRotation(i), lvlSharedData->GetObstacleVisibility(i), lvlSharedData->GetIsObstacleHollow(i));
        mObstacles.push_back(newObstacle);
    }
    mPlayerSpawnTransform.SetPosition(lvlSharedData->GetPlayerPosition());
    mPlayerSpawnTransform.SetDimensions(lvlSharedData->GetPlayerDimensions());
    mPlayerSpawnTransform.SetRotation(lvlSharedData->GetPlayerRotation());

    GameData::GetPtr().GetPathFinder()->GetUnblockedPathPointsInRect(Vector2D(), GameData::GetPtr().GetScreenDimensions(), 0.f, mWalkablePoints);
}

const std::list<std::shared_ptr<PathObstacle>>& Map::GetObstacles() const
{
    return mObstacles;
}

Object::Transform Map::GetPlayerSpawnTransform()
{
    return mPlayerSpawnTransform;
}

void Map::Draw() const
{
    for (std::shared_ptr<PathObstacle> obstacle : mObstacles)
    {
        obstacle->Draw();
    }
}

const std::vector<Vector2D> Map::GetAllWalkablePoints() const
{
    return mWalkablePoints;
}

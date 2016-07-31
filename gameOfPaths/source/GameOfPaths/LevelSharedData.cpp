#include "pch.h"
#include "LevelSharedData.h"

LevelSharedData::LevelSharedData()
{
}

Vector2D LevelSharedData::GetPlayerPosition() const
{
    return mPlayerData.mPosition;
}

Vector2D LevelSharedData::GetPlayerDimensions() const
{
    return mPlayerData.mDimension;
}

float LevelSharedData::GetPlayerRotation() const
{
    return mPlayerData.mRotation;
}

size_t LevelSharedData::GetObstacleCount() const
{
    return mObstacleList.size();
}

Vector2D LevelSharedData::GetObstaclePosition(int index) const
{
    return mObstacleList[index].mPosition;
}

Vector2D LevelSharedData::GetObstacleDimensions(int index) const
{
    return mObstacleList[index].mDimension;
}

float LevelSharedData::GetObstacleRotation(int index) const
{
    return mObstacleList[index].mRotation;
}

bool LevelSharedData::GetPlayerVisibility() const
{
    return mPlayerData.mVisible;
}

bool LevelSharedData::GetObstacleVisibility(int index) const
{
    return mObstacleList[index].mVisible;
}

bool LevelSharedData::GetIsObstacleHollow(int index) const
{
    return mObstacleList[index].mIsHollow;
}


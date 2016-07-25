#pragma once
#include "Library\XmlParseMaster.h"
#include "Library\Grid.h"
#include <tuple>
class LevelSharedData :
    public Library::XmlParseMaster::SharedData
{
    friend class LevelXmlParseHelper;
public:
    LevelSharedData();
    virtual ~LevelSharedData() = default;
    Vector2D GetPlayerPosition() const;
    Vector2D GetPlayerDimensions() const;
    float GetPlayerRotation() const;
    bool GetPlayerVisibility() const;
    size_t GetObstacleCount() const;

    Vector2D GetObstaclePosition(int index) const;
    Vector2D GetObstacleDimensions(int index) const;
    float GetObstacleRotation(int index) const;
    bool GetObstacleVisibility(int index) const;
private:
    struct RenderableData
    {
        Vector2D mPosition;
        Vector2D mDimension;
        float mRotation;
        bool mVisible;
    };
    std::vector<RenderableData> mObstacleList;
    RenderableData mPlayerData;
};
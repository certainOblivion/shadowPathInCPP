#pragma once
#include <list>
#include <memory>
#include "Library\IObject.h"
class PathObstacle;
class Map : public Object::IRenderableObject
{
public:
    Map();
    ~Map();
    void Init();
    const std::list<std::shared_ptr<PathObstacle>>& GetObstacles() const;
    Object::Transform GetPlayerSpawnTransform();
    const std::vector<Vector2D> GetAllWalkablePoints() const;
    virtual void Draw() const override;

private:
    std::vector<Vector2D> mWalkablePoints;
    std::list<std::shared_ptr<PathObstacle>> mObstacles;
    Object::Transform mPlayerSpawnTransform;
};


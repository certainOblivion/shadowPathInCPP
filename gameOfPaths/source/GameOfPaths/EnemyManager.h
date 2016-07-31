#pragma once
#include <vector>
#include "Library\Grid.h"
#include "Enemy.h"
#include "Library\IObject.h"
#include "Map.h"
class EnemyManager : public Object::IRenderableObject, public Object::IUpdatableObject
{
public:
    EnemyManager(int maxEnemies, const Map& map);
    ~EnemyManager() = default;

    void Init();
    void Update(float dT);
    void CreateEnemy();

    virtual void Draw() const override;

private:
    unsigned int mMaxEnemies;
    std::vector<std::shared_ptr<Enemy>> mEnemies;
    const Map* mMap;
};
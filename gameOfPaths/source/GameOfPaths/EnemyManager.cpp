#include "pch.h"
#include "EnemyManager.h"
#include "GameHelper.h"
#include "SFML\Graphics.hpp"
#include <iostream>
using namespace std;

EnemyManager::EnemyManager(int maxEnemies, const Map& map) : mMaxEnemies(maxEnemies), mMap(&map)
{
    mEnemies.reserve(maxEnemies);
    Init();
}

void EnemyManager::Init()
{
    
}

void EnemyManager::Update(float dT)
{

    if (mEnemies.size() < mMaxEnemies)
    {
        CreateEnemy();
    }
    for (shared_ptr<Enemy>& enemy : mEnemies)
    {
        enemy->Update(dT);
    }
}

void EnemyManager::CreateEnemy()
{
    //const std::vector<Vector2D>& walkablePosition = mMap->GetAllWalkablePoints();

//    throw_assert(walkablePosition.size() > 0 , "there are no spawn points");
    const Vector2D& spawnPoint = Vector2D(800,8);//walkablePosition[rand() % walkablePosition.size()];

    std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(spawnPoint, 0.f, Vector2D(10, 10), GameHelper::GetInstance().GetPlayer()->Transform());
    mEnemies.push_back(enemy);
}

void EnemyManager::Draw() const
{
    for (const shared_ptr<Enemy>& enemy : mEnemies)
    {
        enemy->Draw();
    }
}

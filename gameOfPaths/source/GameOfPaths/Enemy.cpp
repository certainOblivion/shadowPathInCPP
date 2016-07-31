#include "pch.h"
#include "Enemy.h"
#include "Library\MathHelper.h"
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#include "GameData.h"
#include "AssetLoader.h"
using namespace sf;
using namespace Helper;
void Enemy::Update(float dT)
{
    if (mPath.get() != nullptr && mPath->size() > 0)
    {
        if (Position() == mPath->front())
        {
            mPath->pop_front();
            if (mPath->size() == 0)
            {
                return;
            }
        }

        Vector2D& newPosition = Helper::MathHelper::MoveTowards(Position(), mPath->front(), 200 * dT);
        SetPosition(newPosition);
        SetRotation(GameData::WorldToScreenRotation(MathHelper::LookAt(mPath->front(), Position())));
    }
    else
    {
        mPath = std::make_shared<std::list<Vector2D>>();
        GameData::GetPtr().GetPathFinder()->GetPath(Position(), GameData::GetPtr().GetPlayer()->Position(), *mPath);
    }
}

Enemy::Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions)
    : mTransform(position, rotation, dimensions), IRenderableObject(true)
{
    unsigned int textureSize = 0;
    const char* textureData = AssetLoader::GetAsset("resources\\zombie.png", textureSize);
    Vector2D screenDimensions = GameData::GetPtr().GetScreenDimensions();
    mTexture.loadFromMemory(textureData, textureSize);
    mTexture.setSmooth(true);
}

void Enemy::SetPath(std::shared_ptr<std::list<Vector2D>> newPath)
{
    mPath = newPath;
}

void Enemy::Draw() const
{
    if (IsVisible())
    {
        sf::Sprite sprite;
        sprite.setTexture(mTexture);
        sprite.setScale(Vector2f(static_cast<float>(Dimensions().x * 7 / mTexture.getSize().x), static_cast<float>(Dimensions().y * 7 / mTexture.getSize().y)));
        Vector2f screenPosition = GameData::WorldToScreenPoint(Position());
        sprite.setPosition(screenPosition);
        sprite.setOrigin(Vector2f(static_cast<float>(sprite.getTextureRect().width / 2), static_cast<float>(sprite.getTextureRect().height / 2)));

        sprite.setRotation(RotationInDegrees());
        GameData::GetPtr().GetWindow()->draw(sprite);
#if !RELEASE
        GameData::DrawHex(Position(), sf::Color::Blue, false);
        Vertex v;
        v.position = screenPosition;
        v.color = Color::Cyan;

        GameData::GetPtr().GetWindow()->draw(&v, 1, sf::Points);
#endif
    }
}

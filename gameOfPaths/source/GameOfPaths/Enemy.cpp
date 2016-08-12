#include "pch.h"
#include "Enemy.h"
#include "Library\MathHelper.h"
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#include "GameHelper.h"
#include "AssetLoader.h"
using namespace sf;
using namespace Helper;
void Enemy::Update(float dT)
{
    const bool hasTargetMoved = mTarget->Position() != mPrevTargetLocation;
    mPrevTargetLocation = mTarget->Position();

    if (hasTargetMoved && GameHelper::GetPtr().GetPathFinder()->RayTrace(Position(), mTarget->Position()))
    {
        mPath->clear();
        mPath->push_back(mTarget->Position());
    }
    else if (mPath->size() > 0)
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
        //SetPosition(newPosition);
        SetRotation(GameHelper::WorldToScreenRotation(MathHelper::LookAt(mPath->front(), Position())));

        if (hasTargetMoved)
        {
            GameHelper::GetPtr().GetPathFinder()->UpdatePathToMovingTarget(*mPath, mTarget->Position(), Position());
        }
    }
    else
    {
        GameHelper::GetPtr().GetPathFinder()->GetPath(
            Position()
            , mTarget->Position()
            , *mPath
#if !RELEASE
            , &mHexInPath
            , &mTestedHex
            , &mTimeToFindPath
#endif
            );
    }
}

Enemy::Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions, const Object::Transform& target)
    : mTransform(position, rotation, dimensions), IRenderableObject(true), mTarget(&target), mPath(std::make_shared<std::list<Vector2D>>())
{
    unsigned int textureSize = 0;
    const char* textureData = AssetLoader::GetAsset("resources\\zombie.png", textureSize);
    Vector2D screenDimensions = GameHelper::GetPtr().GetScreenDimensions();
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
        Vector2f screenPosition = GameHelper::WorldToScreenPoint(Position());
        sprite.setPosition(screenPosition);
        sprite.setOrigin(Vector2f(static_cast<float>(sprite.getTextureRect().width / 2), static_cast<float>(sprite.getTextureRect().height / 2)));

        sprite.setRotation(RotationInDegrees());
        GameHelper::GetPtr().GetWindow()->draw(sprite);
#if !RELEASE
        GameHelper& gameInfo = GameHelper::GetPtr();
        for (const Grid::Hex& hex : mTestedHex)
        {
            gameInfo.DrawHex(hex, sf::Color::Blue);
        }
        for (const Grid::Hex& hex : mHexInPath)
        {
            gameInfo.DrawHex(hex, sf::Color::Cyan);
        }

        for (const Vector2D& pos : *mPath)
        {
            gameInfo.DrawHex(pos, sf::Color::Green);
        }
        GameHelper::DrawHex(Position(), sf::Color::Blue, false);
        Vertex v;
        v.position = screenPosition;
        v.color = Color::Cyan;

        GameHelper::GetPtr().GetWindow()->draw(&v, 1, sf::Points);
#endif
    }
}

void Enemy::SetTarget(const Object::Transform& target)
{
    mTarget = &target;
}

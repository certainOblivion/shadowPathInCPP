#include "pch.h"
#include "Enemy.h"
#include "Library\MathHelper.h"
#include "SFML\Graphics.hpp"
#include "SFML\System.hpp"
#include "GameHelper.h"
#include "AssetLoader.h"
#include "PathFinderManager.h"
#include <functional>
using namespace sf;
using namespace Helper;
void Enemy::Update(float dT)
{
    const bool hasTargetMoved = mTarget->Position() != mPrevTargetLocation;
    mPrevTargetLocation = mTarget->Position();

    /*if (hasTargetMoved && GameHelper::GetInstance().GetPathFinder()->RayTrace(Position(), mTarget->Position()))
    {
        mPath->clear();
        mPath->push_back(mTarget->Position());
    }
    else*/ if (mPathReceived && mPath->size() > 0)
    {
        if (Position() == mPath->front())
        {
            mPath->pop_front();
            if (mPath->size() == 0)
            {
                ResetPath();
                return;
            }
        }

        Vector2D newPosition = Helper::MathHelper::MoveTowards(Position(), mPath->front(), 200 * dT);
        SetPosition(newPosition);
        SetRotation(GameHelper::WorldToScreenRotation(MathHelper::LookAt(mPath->front(), Position())));

         if (hasTargetMoved)
         {
             UpdatePathToMovingTarget(*mPath, mTarget->Position(), Position());
         }
    }
    else
    {
        if (!mPathRequested)
        {
             PathFinderManager::RequestPath(
                 Position()
                 , mTarget->Position()
                 , std::bind(&Enemy::SetPath, this, std::placeholders::_1, std::placeholders::_2)
             );
            mPathRequested = true;
        }
    }
}

Enemy::Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions, const Object::Transform& target)
    : mTransform(position, rotation, dimensions), IRenderableObject(true), mTarget(&target), mPath(std::make_shared<std::list<Vector2D>>()) , mPathRequested(false), mPathReceived(false)
{
    unsigned int textureSize = 0;
    const char* textureData = AssetLoader::GetAsset("resources\\zombie.png", textureSize);
    Vector2D screenDimensions = GameHelper::GetInstance().GetScreenDimensions();
    mTexture.loadFromMemory(textureData, textureSize);
    mTexture.setSmooth(true);
}

void Enemy::UpdatePathToMovingTarget(std::list<Vector2D> &path, const Vector2D& newTarget, const Vector2D& currentPosition)
{
    //make sure the newTarget is not the same as the oldTarget
    if (*path.rbegin() == newTarget)
    {
        return;
    }
    //find closest node in the path to the newTarget
    auto it = path.crbegin();
    for (it; it != path.crend(); ++it)
    {
        auto next = std::next(it, 1);
        if (next != path.crend())
        {
            if (Vector2D::SqrMagnitude(newTarget, (*next)) > Vector2D::SqrMagnitude(newTarget, (*it)))
            {
                if (it != path.crbegin())
                {
                    --it;
                }
                break;
            }
        }
    }

    auto pathEnd = path.crbegin().base();
    auto deleteIt = it.base();

    //put the newTarget as the next node to the closest found, delete the rest
    path.erase(deleteIt, pathEnd);
    std::list<Vector2D> deltaPath;

    Vector2D deltaPathBegin = path.size() == 0 ? currentPosition : *path.rbegin();

    auto joinReceivedPath = [this](std::list<Vector2D> newPath, bool pathFound)
    {
        if (pathFound)
        {
            for (Vector2D& waypoint : newPath)
            {
                mPath->push_back(waypoint);
            }

            PathFinderManager::RequestPathSmoothing(
                *mPath
                , std::bind(&Enemy::SetPath, this, std::placeholders::_1, std::placeholders::_2)
                );
        }
        else
        {
            ResetPath();
        }
    };

    PathFinderManager::RequestPath(
        deltaPathBegin
        , newTarget
        , joinReceivedPath
        );
}

void Enemy::SetPath(std::list<Vector2D> newPath, bool pathFound)
{
    std::lock_guard<std::mutex> lockGuard(mMutex);
    if (pathFound)
    {
        *mPath = newPath;
        mPathReceived = true;
    }
    else
    {
        ResetPath();
    }
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
        GameHelper::GetInstance().GetWindow()->draw(sprite);
#if !RELEASE
        GameHelper& gameInfo = GameHelper::GetInstance();
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

        GameHelper::GetInstance().GetWindow()->draw(&v, 1, sf::Points);
#endif
    }
}

void Enemy::SetTarget(const Object::Transform& target)
{
    mTarget = &target;
}

void Enemy::ResetPath()
{
    mPath->clear();
    mPathReceived = false;
    mPathRequested = false;
}

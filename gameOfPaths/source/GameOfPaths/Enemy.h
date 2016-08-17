#pragma once
#include "Library\IObject.h"
#include "SFML\Graphics.hpp"
#include<mutex>

class Enemy : public Object::IRenderableObject, public Object::IUpdatableObject
{
public:
    Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions, const Object::Transform& target);
    virtual ~Enemy() = default;
    void SetTarget(const Object::Transform& target);
    void SetPath(std::list<Vector2D> newPath, bool pathFound);
    virtual void Update(float dT) override;
    /** Updates the path to the target assuming that the target has moved from the original place
    * Algorithm assumes the delta between the last position and the current position is small (locomotion not teleportation)
    * @param path calculated to the target's original position
    * @param target's new position
    */
    void UpdatePathToMovingTarget(std::list<Vector2D> &path, const Vector2D& newTarget, const Vector2D& currentPosition);
    virtual void Draw() const override;
    _TRANSFORM_AS_COMPOSITION;
private:
    void ResetPath();
    std::shared_ptr<std::list<Vector2D>> mPath;
    sf::Texture mTexture;
    const Object::Transform* mTarget;
    Vector2D mPrevTargetLocation;
    bool mPathRequested;
    bool mPathReceived;
    std::mutex mMutex;
#if !RELEASE
    std::list<Grid::Hex> mHexInPath;
    std::unordered_set<Grid::Hex> mTestedHex;
    long mTimeToFindPath;
#endif
};


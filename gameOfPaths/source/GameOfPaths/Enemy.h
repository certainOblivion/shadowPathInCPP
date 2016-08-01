#pragma once
#include "Library\IObject.h"
#include "SFML\Graphics.hpp"
class Enemy : public Object::IRenderableObject, public Object::IUpdatableObject
{
public:
    Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions, const Object::Transform& target);
    virtual ~Enemy() = default;
    void SetTarget(const Object::Transform& target);
    void SetPath(std::shared_ptr<std::list<Vector2D>> newPath);
    virtual void Update(float dT) override;

    virtual void Draw() const override;
    _TRANSFORM_AS_COMPOSITION;
private:
    std::shared_ptr<std::list<Vector2D>> mPath;
    sf::Texture mTexture;
    const Object::Transform* mTarget;

#if !RELEASE
    std::list<Grid::Hex> mHexInPath;
    std::unordered_set<Grid::Hex> mTestedHex;
    long mTimeToFindPath;
#endif
};


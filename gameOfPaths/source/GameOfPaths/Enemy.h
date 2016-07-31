#pragma once
#include "Library\IObject.h"
#include "SFML\Graphics.hpp"
class Enemy : public Object::IRenderableObject, public Object::IUpdatableObject
{
public:
    Enemy(const Vector2D& position, float rotation, const Vector2D& dimensions);
    virtual ~Enemy() = default;
    void SetPath(std::shared_ptr<std::list<Vector2D>> newPath);
    virtual void Update(float dT) override;

    virtual void Draw() const override;
    _TRANSFORM_AS_COMPOSITION;
private:
    std::shared_ptr<std::list<Vector2D>> mPath;
    sf::Texture mTexture;
};


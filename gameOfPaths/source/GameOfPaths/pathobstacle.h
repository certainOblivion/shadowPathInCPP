#pragma once
#include "Library\IObject.h"
#include "GameData.h"

class PathObstacle : public Object::IRenderableObject
{
public:
    PathObstacle(const Vector2D& position, float rotation, const Vector2D& dimensions, bool isVisible, bool isHollow = false) : mTransform(position, rotation, dimensions), IRenderableObject(isVisible) {}
    virtual ~PathObstacle() = default;

    bool IsHollow() const { return mIsHollow; }

    _TRANSFORM_AS_COMPOSITION;

    virtual void Draw() const override
    {
        if (IsVisible())
        {
            sf::RectangleShape shape(sf::Vector2f(static_cast<float>(mTransform.Dimensions().x), static_cast<float>(mTransform.Dimensions().y)));
            sf::Vector2f screenPosition = GameData::WorldToScreenPoint(mTransform.Position());
            shape.setPosition(screenPosition);
            shape.setRotation(-mTransform.RotationInDegrees());
            shape.setOrigin(sf::Vector2f(static_cast<float>(mTransform.Dimensions().x / 2), static_cast<float>(mTransform.Dimensions().y / 2)));
            shape.setFillColor(sf::Color(153, 0, 51));
            GameData::GetPtr().GetWindow()->draw(shape);
        }
    }

private:
    bool mIsHollow;
    
};
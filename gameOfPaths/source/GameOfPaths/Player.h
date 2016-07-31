#pragma once
#include "Library\Grid.h"
#include "Library\IObject.h"
#include "SFML\Graphics.hpp"
class Player : public Object::IRenderableObject, public Object::IUpdatableObject
{
    _TRANSFORM_AS_COMPOSITION;
    friend struct std::hash<Player>;
public:
    Player(const Vector2D& position, float rotation, const Vector2D& dimensions);
    virtual ~Player();
    void Update(float dT) override;
    virtual void Draw() const override;

private:
    Player() = delete;
    sf::Texture mPlayerTexture;
};


namespace std
{
    template<>
    struct hash<Player>
    {
    public:
        std::size_t operator()(const Player& h) const
        {
            using std::size_t;
            using std::hash;

            return (hash<Vector2D>()(h.Position())
                ^ (hash<float>()(h.RotationInDegrees()) << 1))
                ^ (hash<Vector2D>()(h.Dimensions()) << 2);
        }
    };
}


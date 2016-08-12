#include "pch.h"
#include "Player.h"
#include "SFML\Graphics.hpp"
#include "GameData.h"
#include "AssetLoader.h"
#include "Library\MathHelper.h"
using namespace sf;
using namespace Helper;
Player::~Player()
{
}

void Player::Update(float dT)
{
    Vector2D newPos = Position();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        Vector2D forward(sin(RotationInRadians()), cos(RotationInRadians()));
        newPos = Position() + forward * 250 * dT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        Vector2D forward(sin(RotationInRadians()), cos(RotationInRadians()));
        newPos = Position() - forward * 250 * dT;
    }

    SetPosition(newPos);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        SetRotation(RotationInDegrees() - 5);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        SetRotation(RotationInDegrees() + 5);
    }
}

Player::Player(const Vector2D& position, float rotation, const Vector2D& dimensions) : mTransform(position, rotation, dimensions), IRenderableObject( true)
{
    unsigned int textureSize = 0;
    const char* textureData = AssetLoader::GetAsset("resources\\hero.png", textureSize);
    Vector2D screenDimensions = GameData::GetPtr().GetScreenDimensions();
    mPlayerTexture.loadFromMemory(textureData, textureSize);
    mPlayerTexture.setSmooth(true);
}

void Player::Draw() const
{
    if (IsVisible())
    {
        sf::Sprite sprite;
        sprite.setTexture(mPlayerTexture);
        sprite.setScale(Vector2f(static_cast<float>(Dimensions().x * 7/ mPlayerTexture.getSize().x), static_cast<float>(Dimensions().y *7/ mPlayerTexture.getSize().y)));
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


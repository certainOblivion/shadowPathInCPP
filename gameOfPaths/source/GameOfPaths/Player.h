#pragma once
#include "Library\Grid.h"
#include "Library\IObject.h"
class Player : public Object::IRenderableObject, public Object::IUpdatableObject
{
	friend struct std::hash<Player>;
public:
	Player(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc);
	virtual ~Player();
	void Update(float dT) override;
private:
	Player() = delete;
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


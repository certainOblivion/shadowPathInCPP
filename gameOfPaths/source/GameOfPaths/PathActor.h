#pragma once
#include "IRenderableObject.h"
#include "Grid.h"
#include "IUpdatableObject.h"
class PathActor : public IRenderableObject, public IUpdatableObject
{
	friend struct std::hash<PathActor>;
public:
	PathActor(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc) : IRenderableObject(position, rotation, dimensions, drawFunc) {}
	~PathActor();
	void Draw() override;
	void Update(float dT) override;
private:
	PathActor() = delete;
};


namespace std
{
	template<>
	struct hash<PathActor>
	{
	public:
		std::size_t operator()(const PathActor& h) const
		{
			using std::size_t;
			using std::hash;

			return (hash<Vector2D>()(h.Position())
				^ (hash<float>()(h.Rotation()) << 1))
				^ (hash<Vector2D>()(h.Dimensions()) << 2);
		}
	};
}


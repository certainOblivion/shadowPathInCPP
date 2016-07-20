#pragma once
#include "Grid.h"
#include <functional>

class IRenderableObject
{
public:
	Vector2D Position()const { return mPosition; }
	float Rotation() const { return mRotation; }
	Vector2D Dimensions() const {	return mDimensions;	}

	void SetPosition(const Vector2D& position) {  mPosition = position; }
	void SetRotation(float rotation)  { mRotation = rotation; }
	void SetDimensions(const Vector2D& dimensions) { mDimensions = dimensions; }
	virtual void Draw() {
		if (mDrawFunction) { mDrawFunction(mPosition, mDimensions, mRotation); }
	}
	IRenderableObject(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc) : mPosition(position), mRotation(rotation), mDimensions(dimensions), mDrawFunction(*drawFunc) {}

protected:
	IRenderableObject() = delete;
	Vector2D mPosition;
	float mRotation;
	Vector2D mDimensions;
private:
	std::function<void(Vector2D, Vector2D, float)> mDrawFunction;

};

namespace std
{
	template<>
	struct hash<IRenderableObject>
	{
	public:
		std::size_t operator()(const IRenderableObject& h) const
		{
			using std::size_t;
			using std::hash;

			return (hash<Vector2D>()(h.Position())
				^ (hash<float>()(h.Rotation()) << 1))
				^ (hash<Vector2D>()(h.Dimensions()) << 2);
		}
	};
}
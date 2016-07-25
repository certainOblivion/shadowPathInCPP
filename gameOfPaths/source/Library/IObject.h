#pragma once
#include "Grid.h"
#include <functional>
#include "MathHelper.h"

namespace Object
{
	class IObject
	{};

	class IRenderableObject : virtual public IObject
	{
	public:
		virtual Vector2D Position()const { return mPosition; }
		virtual float RotationInDegrees() const { return mRotation; }
		virtual float RotationInRadians() const { return mRotation * Helper::MathHelper::Deg2Rad; }
		virtual void SetPosition(const Vector2D& position) { mPosition = position; }
		virtual void SetRotation(float rotation) { mRotation = rotation; }
		virtual Vector2D Dimensions() const { return mDimensions; }
		virtual void SetDimensions(const Vector2D& dimensions) { mDimensions = dimensions; }
        virtual void SetVisibility(bool visibility) { mIsVisible = visibility; }
        virtual bool IsVisible() { return mIsVisible ; }
		virtual void Draw() const {
			if (mDrawFunction != nullptr && mIsVisible) { mDrawFunction(mPosition, mDimensions, mRotation); }
		}
		IRenderableObject(const Vector2D& position, float rotation, const Vector2D& dimensions, bool isVisible = true, std::function<void(Vector2D, Vector2D, float)>* drawFunc = nullptr) 
            : mPosition(position),mRotation(rotation), mDimensions(dimensions),mIsVisible(isVisible), mDrawFunction(*drawFunc) 
        {}
        virtual ~IRenderableObject() = default;
	protected:
		IRenderableObject() = delete;

		Vector2D mPosition;
		float mRotation;
		Vector2D mDimensions;
		std::function<void(Vector2D, Vector2D, float)> mDrawFunction;
        bool mIsVisible;
	};

	class IUpdatableObject : virtual public IObject
	{
	public:
		virtual void Update(float dT) = 0;
        virtual ~IUpdatableObject() = default;
	};
}

namespace std
{
	template<>
	struct hash<Object::IRenderableObject>
	{
	public:
		std::size_t operator()(const Object::IRenderableObject& h) const
		{
			using std::size_t;
			using std::hash;

			return (hash<Vector2D>()(h.Position())
				^ (hash<float>()(h.RotationInDegrees()) << 1))
				^ (hash<Vector2D>()(h.Dimensions()) << 2);
		}
	};
}
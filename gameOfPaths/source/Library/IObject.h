#pragma once
#include "Grid.h"
#include <functional>
#include "MathHelper.h"

namespace Object
{
    class Transform
    {
    public:
        Transform()
            :mPosition(Vector2D()), mDimensions(Vector2D()), mRotation(0.f)
        {}
        Transform(const Vector2D& position, float rotation, const Vector2D& dimensions)
            :mPosition(position), mDimensions(dimensions), mRotation(rotation)
        {}
        virtual Vector2D Position()const { return mPosition; }
        virtual float RotationInDegrees() const { return mRotation; }
        virtual float RotationInRadians() const { return mRotation * Helper::MathHelper::Deg2Rad; }
        virtual void SetPosition(const Vector2D& position) { mPosition = position; }
        virtual void SetRotation(float rotation) { mRotation = rotation; }
        virtual Vector2D Dimensions() const { return mDimensions; }
        virtual void SetDimensions(const Vector2D& dimensions) { mDimensions = dimensions; }

    protected:
        Vector2D mPosition;
        float mRotation;
        Vector2D mDimensions;
    };

    class IRenderableObject
    {
    public:
        virtual void Draw() const = 0;
        virtual void SetVisibility(bool visibility) { mIsVisible = visibility; }
        virtual bool IsVisible() const { return mIsVisible; }
        IRenderableObject()
            : mIsVisible(true)
        {}
        IRenderableObject(bool isVisible)
            :mIsVisible(isVisible)
        {}
        virtual ~IRenderableObject() = default;
    protected:
        bool mIsVisible;
    };

    class IUpdatableObject
    {
    public:
        virtual void Update(float dT) = 0;
        virtual ~IUpdatableObject() = default;
    };
}

namespace std
{
    template<>
    struct hash<Object::Transform>
    {
    public:
        std::size_t operator()(const Object::Transform& h) const
        {
            using std::size_t;
            using std::hash;

            return (hash<Vector2D>()(h.Position())
                ^ (hash<float>()(h.RotationInDegrees()) << 1))
                ^ (hash<Vector2D>()(h.Dimensions()) << 2);
        }
    };
}

//add an mTransform member of a class
#define _TRANSFORM_AS_COMPOSITION \
private:                        \
Object::Transform mTransform;   \
public:                         \
Vector2D Position()const { return mTransform.Position(); }\
float RotationInDegrees() const { return mTransform.RotationInDegrees(); }\
float RotationInRadians() const { return mTransform.RotationInRadians() ; }\
void SetPosition(const Vector2D& position) { mTransform.SetPosition(position); }\
void SetRotation(float rotation) { mTransform.SetRotation (rotation); }\
Vector2D Dimensions() const { return mTransform.Dimensions(); }\
void SetDimensions(const Vector2D& dimensions) { mTransform.SetDimensions (dimensions); }\


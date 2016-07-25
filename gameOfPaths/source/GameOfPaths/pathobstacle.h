#pragma once
#include "Library\IObject.h"

class PathObstacle : public Object::IRenderableObject
{
public:
	PathObstacle(const Vector2D& position, float rotation, const Vector2D& dimensions,bool isVisible, std::function<void(Vector2D, Vector2D, float)>* drawFunc) : IRenderableObject(position, rotation, dimensions,isVisible, drawFunc) {}
	virtual ~PathObstacle() = default;
};


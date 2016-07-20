#pragma once
#include "Library\IObject.h"

class PathObstacle : public Object::IRenderableObject
{
public:
	PathObstacle(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc) : IRenderableObject(position, rotation, dimensions,drawFunc) {}
	~PathObstacle();
};


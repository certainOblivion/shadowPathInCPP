#pragma once
#include "IRenderableObject.h"
class PathObstacle : public IRenderableObject
{
public:
	PathObstacle(const Vector2D& position, float rotation, const Vector2D& dimensions, std::function<void(Vector2D, Vector2D, float)>* drawFunc) : IRenderableObject(position, rotation, dimensions,drawFunc) {}
	~PathObstacle();
};


#pragma once
#include "Grid.h"
class Rect
{
public:
	Rect(Vector2D center, Vector2D dimensions);
	~Rect();

private:
	Vector2D mCenter;
	Vector2D mDimensions;
};


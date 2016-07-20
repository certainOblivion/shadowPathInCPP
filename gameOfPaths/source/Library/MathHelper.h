#pragma once
#include "Grid.h"

namespace Helper
{
	static Vector2D MoveTowards(Vector2D current, Vector2D target, float maxDistanceDelta)
	{
		Vector2D a = target - current;
		double magnitude = a.Magnitude();
		if (magnitude <= maxDistanceDelta || magnitude == 0)
		{
			return target;
		}
		return current + a / magnitude * maxDistanceDelta;
	}
}

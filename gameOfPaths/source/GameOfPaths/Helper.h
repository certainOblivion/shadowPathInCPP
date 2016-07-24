#pragma once
#include "SFML\System.hpp"
#include "Library\Grid.h"
namespace Helper
{
	static Vector2D ScreenDimensions = Vector2D(1900,1200);
	static sf::Vector2f WorldToScreenPoint(const Vector2D& worldPoint)
	{
		double x = worldPoint.x;
		double y = worldPoint.y;

		x += (ScreenDimensions.x / 2);
		y = ScreenDimensions.y - y - (ScreenDimensions.y / 2);

		return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
	}

	static Vector2D ScreenToWorldPoint(const sf::Vector2f& screenPoint)
	{
		float x = screenPoint.x;
		float y = screenPoint.y;

		x-= (static_cast<float>(ScreenDimensions.x) / 2);
		y = static_cast<float>(ScreenDimensions.y) - y - (static_cast<float>(ScreenDimensions.y) / 2);

		return Vector2D(static_cast<double>(x), static_cast<double>(y));
	}
}
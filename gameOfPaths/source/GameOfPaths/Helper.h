#pragma once
#include "SFML\System.hpp"
#include "Library\Grid.h"
#include <shlwapi.h>
namespace Helper
{
    static Vector2D ScreenDimensions = Vector2D(1900, 1200);

    sf::Vector2f WorldToScreenPoint(const Vector2D& worldPoint);

    Vector2D ScreenToWorldPoint(const sf::Vector2f& screenPoint);

    std::wstring ExecutableDirectory();
}
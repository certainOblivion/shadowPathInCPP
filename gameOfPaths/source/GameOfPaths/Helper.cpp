#include "pch.h"
#include "Helper.h"

//////////////////////////////////////////////////////////////////////////
sf::Vector2f Helper::WorldToScreenPoint(const Vector2D& worldPoint)
{
    double x = worldPoint.x;
    double y = worldPoint.y;

    x += (ScreenDimensions.x / 2);
    y = ScreenDimensions.y - y - (ScreenDimensions.y / 2);

    return sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}

//////////////////////////////////////////////////////////////////////////
Vector2D Helper::ScreenToWorldPoint(const sf::Vector2f& screenPoint)
{
    float x = screenPoint.x;
    float y = screenPoint.y;

    x -= (static_cast<float>(ScreenDimensions.x) / 2);
    y = static_cast<float>(ScreenDimensions.y) - y - (static_cast<float>(ScreenDimensions.y) / 2);

    return Vector2D(static_cast<double>(x), static_cast<double>(y));
}

//////////////////////////////////////////////////////////////////////////
std::wstring Helper::ExecutableDirectory()
{
    WCHAR buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    PathRemoveFileSpec(buffer);

    return std::wstring(buffer);
}


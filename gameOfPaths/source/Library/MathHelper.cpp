#include "MathHelper.h"
#include <iostream>

using namespace Helper;

const double MathHelper::Pi = 3.14159265359;
const float Helper::MathHelper::TwoPi = 6.28318530718f;
const float Helper::MathHelper::PiOver4 = 0.78539816339f;
const float Helper::MathHelper::Sqrt2 = 1.41421356237f;
const double MathHelper::Sqrt3 = 1.73205080757;
const float Helper::MathHelper::Deg2Rad = 0.0174532924F;
const float Helper::MathHelper::Rad2Deg = 57.29578F;

//////////////////////////////////////////////////////////////////////////
Vector2D Helper::MathHelper::MoveTowards(Vector2D current, Vector2D target, float maxDistanceDelta)
{
    Vector2D a = target - current;
    double magnitude = a.Magnitude();
    if (magnitude <= maxDistanceDelta || magnitude == 0)
    {
        return target;
    }
    return current + a / magnitude * maxDistanceDelta;
}

//////////////////////////////////////////////////////////////////////////
void Helper::MathHelper::GetRectangleCorners(const Vector2D& center, const Vector2D& dimensions, float rotationInRadians, std::vector<Vector2D>& corners)
{
    corners.clear();
    corners.reserve(4);

    //clockwise from topleft
    Vector2D topLeft(center.x - (dimensions.x / 2), center.y + (dimensions.y / 2));
    Vector2D topRight(center.x + (dimensions.x / 2), center.y + (dimensions.y / 2));
    Vector2D bottomRight(center.x + (dimensions.x / 2), center.y - (dimensions.y / 2));
    Vector2D bottomLeft(center.x - (dimensions.x / 2), center.y - (dimensions.y / 2));

    corners.emplace_back(RotatePoint(topLeft, center, rotationInRadians));
    corners.emplace_back(RotatePoint(topRight, center, rotationInRadians));
    corners.emplace_back(RotatePoint(bottomRight, center, rotationInRadians));
    corners.emplace_back(RotatePoint(bottomLeft, center, rotationInRadians));
}

//////////////////////////////////////////////////////////////////////////
Vector2D Helper::MathHelper::RotatePoint(const Vector2D& point, const Vector2D& rotateAround, float angleInRadians)
{
    if (angleInRadians == 0)
    {
        return point;
    }

    float tempX = static_cast<float>(point.x - rotateAround.x);
    float tempY = static_cast<float>(point.y - rotateAround.y);

    // now apply rotation
    float rotatedX = tempX * cos(angleInRadians) - tempY * sin(angleInRadians);
    float rotatedY = tempX * sin(angleInRadians) + tempY * cos(angleInRadians);

    // translate back
    Vector2D rotatedPoint(rotatedX + rotateAround.x, rotatedY + rotateAround.y);

    return rotatedPoint;
}

//////////////////////////////////////////////////////////////////////////
bool Helper::MathHelper::LeftOf(const Vector2D& p1, const Vector2D& p2, const Vector2D& point)
{
    double cross = (p2.x - p1.x) * (point.y - p1.y) - (p2.y - p1.y) * (point.x - p1.x);

    return cross < 0;
}

//////////////////////////////////////////////////////////////////////////
Vector2D Helper::MathHelper::Interpolate(const Vector2D& p, const Vector2D& q, float f)
{
    return Vector2D(p.x * (1.0f - f) + q.x * f, p.y * (1.0f - f) + q.y * f);
}

//////////////////////////////////////////////////////////////////////////
Vector2D Helper::MathHelper::LineLineIntersection(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Vector2D& p4)
{
    double s = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x))
        / ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

    return Vector2D(p1.x + s * (p2.x - p1.x), p1.y + s * (p2.y - p1.y));
}

Vector2D Helper::MathHelper::PointProjection(const Vector2D& origin, const Vector2D& pointTowardsDir, float projectionLength)
{
    return origin + ((pointTowardsDir - origin) / (pointTowardsDir - origin).Magnitude() * projectionLength);
}

//////////////////////////////////////////////////////////////////////////
float Helper::MathHelper::LookAt(const Vector2D& target, const Vector2D& currentPosition)
{
    Vector2D dir = currentPosition - target;
    float rotation = static_cast<float>(atan2(dir.y, dir.x)) * Rad2Deg;
    return rotation;
}

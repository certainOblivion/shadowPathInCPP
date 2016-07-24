#ifndef MATH_HELPER
#define MATH_HELPER
#include "Grid.h"

namespace Helper
{
    class MathHelper
    {
    public:
        static const double Pi;
        static const float TwoPi;
        static const float PiOver4;

        static const float Sqrt2;
        static const double Sqrt3;
        static const float Deg2Rad;
        static const float Rad2Deg;

        //returns a point that is a "maxDistanceDelta" distance from "current" towards "target"
        static Vector2D MoveTowards(Vector2D current, Vector2D target, float maxDistanceDelta);

        //populates "corners" with the corners of the rectangle of "center" midpoint and "dimensions" and rotated by "rotationInRadians"
        static void GetRectangleCorners(const Vector2D& center, const Vector2D& dimensions, float rotationInRadians, std::vector<Vector2D>& corners);

        //returns a point which is "point" rotated around "rotateAround" by "angleInRadians" radians
        static Vector2D RotatePoint(const Vector2D& point, const Vector2D& rotateAround, float angleInRadians);

        //returns whether point is on the left of the line formed by p1 and p2
        static bool LeftOf(const Vector2D& p1, const Vector2D& p2, const Vector2D& point);

        // returns a slightly shortened version of the vector: p * (1 - f) + q * f
        static Vector2D Interpolate(const Vector2D& p, const Vector2D& q, float f);

        //Computes the intersection point of the line p1-p2 with p3-p4
        static Vector2D LineLineIntersection(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Vector2D& p4);

        // returns a vector that 
        static Vector2D PointProjection(const Vector2D& origin, const Vector2D& pointTowardsDir, float projectionLength);
    };
}

#endif // MATH_HELPER
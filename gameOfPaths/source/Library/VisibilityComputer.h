#pragma once
#include <list>
#include "Grid.h"
namespace Visibility
{
    class Segment;
#pragma region LineEndPoint
    struct EndPoint : public Vector2D
    {
        EndPoint(const Vector2D& position, bool isStart, Segment* parent, float angle);
        EndPoint(const Vector2D& position, Segment* parent);
        EndPoint(const EndPoint& other);
        static bool Equals(const EndPoint& a, const EndPoint& b);
        bool operator==(const EndPoint& other)const;
        bool operator!=(const EndPoint& other)const { return !(operator==(other)); }
        EndPoint& operator=(const EndPoint& other);
        /*static*/ bool operator<(/*const LineEndPoint& lhs,*/ const EndPoint& rhs);
        float mAngle;
        Segment* mParent;
        bool mIsStart;
    };
#pragma endregion

#pragma region Line

    class Segment
    {
        friend struct std::hash<Segment>;
    public:
        Segment();
        Segment(EndPoint& p1, EndPoint& p2);
        Segment(const Segment& other);
        static bool Equals(const Segment& a, const Segment& b);
        bool operator==(const Segment& other)const;
        bool operator!=(const Segment& other)const { return !(operator==(other)); }
        Segment& operator=(const Segment& other);
        EndPoint* mPoint1;
        EndPoint* mPoint2;
    };

#pragma endregion


#pragma region VisibilityComputer
    class VisibilityComputer
    {
    public:
        VisibilityComputer(const Vector2D& origin, float radius);
        ~VisibilityComputer() = default;
        void LoadBoundaries();
        void AddRectangleOccluder(const Vector2D& position, const Vector2D& dimensions, float rotation);
        void AddLineOccluder(const Vector2D& p1, const Vector2D& p2);
        void ClearOccluders();
        Vector2D Origin() const { return mOrigin; }
        void SetOrigin(const Vector2D& origin) { mOrigin = origin; }
        void GetVisibility(std::vector<Vector2D>& visibleEndPoints);
        const float Radius() { return mRadius; }
        void SetRadius(float radius) { mRadius = radius; }
    private:
        void AddSegment(const Vector2D& p1, const Vector2D& p2);
        void UpdateSegments();
        bool SegmentInFrontOf(const Segment& a, const Segment& b, const Vector2D& relativeTo);
        void AddTriangle(std::vector<Vector2D>& triangles, float angle1, float angle2, Segment* segment);
        std::list<std::shared_ptr<EndPoint>> mEndPoints;
        std::list<std::shared_ptr<Segment>> mLineSegments;
        Vector2D mOrigin;
        float mRadius;
    };
#pragma endregion

}

namespace std
{
#pragma region Hasher
    template<>
    struct hash<Visibility::Segment>
    {
    public:
        std::size_t operator()(const Visibility::Segment& line) const
        {
            using std::size_t;
            using std::hash;

            return ((hash<Vector2D>()(*line.mPoint1)
                ^ (hash<Vector2D>()(*line.mPoint2) << 1)));
        }
    };
    template<>
    struct less<Visibility::EndPoint>
    {
    public:
        bool operator()(const Visibility::EndPoint& lhs, const Visibility::EndPoint& rhs) const
        {
            if (lhs.mAngle > rhs.mAngle) { return false; }
            if (lhs.mAngle < rhs.mAngle) { return true; }

            //if angles are same prefer start points
            if (!lhs.mIsStart && rhs.mIsStart) { return false; }
            if (lhs.mIsStart && !rhs.mIsStart) { return true; }
        }
    };
#pragma endregion Hasher
}


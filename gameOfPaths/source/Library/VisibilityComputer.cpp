#include "VisibilityComputer.h"
#include "MathHelper.h"
#include <algorithm>

using namespace Visibility;
using namespace Helper;
using namespace Grid;
using namespace std;

#pragma region VisibilityComputer

//////////////////////////////////////////////////////////////////////////
Visibility::VisibilityComputer::VisibilityComputer(const Vector2D& origin, float radius) :
    mOrigin(origin), mRadius(radius)
{
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::LoadBoundaries()
{
    AddRectangleOccluder(mOrigin, Vector2D(mRadius, mRadius), 0.f);
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::AddRectangleOccluder(const Vector2D& position, const Vector2D& dimensions, float rotation)
{
    std::vector<Vector2D> corners;
    MathHelper::GetRectangleCorners(position, dimensions, rotation, corners);

    AddSegment(corners[0], corners[1]);
    AddSegment(corners[1], corners[2]);
    AddSegment(corners[2], corners[3]);
    AddSegment(corners[3], corners[0]);
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::AddLineOccluder(const Vector2D& p1, const Vector2D& p2)
{
    AddSegment(p1, p2);
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::ClearOccluders()
{
    mLineSegments.clear();
    mEndPoints.clear();

    LoadBoundaries();
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::GetVisibility(std::vector<Vector2D>& visibleEndPoints)
{
    visibleEndPoints.clear();

    list<Segment*> open;
    UpdateSegments();

    mEndPoints.sort(
        [](const shared_ptr<EndPoint>& a, const shared_ptr<EndPoint>& b)
    {
        return *(a.get()) < *(b.get());;
    });

    float currentAngle = 0.f;

    for (int pass = 0; pass < 2; pass++)
    {
        for (shared_ptr<EndPoint>& p : mEndPoints)
        {
            Segment* currentOld = open.size() == 0 ? nullptr : open.front();
            if (p->mIsStart)
            {
                auto node = open.begin();

                while (node != open.end() && SegmentInFrontOf(*p->mParent, **node, mOrigin))
                {
                    node = std::next(node, 1);
                }

                if (node == open.end())
                {
                    open.push_back(p->mParent);
                }
                else
                {
                    open.insert(node, p->mParent);
                }
            }
            else
            {
                open.remove(p->mParent);
            }

            Segment* currentNew = nullptr;

            if (open.size() != 0)
            {
                currentNew = open.front();
            }

            if (currentOld != currentNew)
            {
                if (pass == 1)
                {
                    AddTriangle(visibleEndPoints, currentAngle, p->mAngle, currentOld);
                }
                currentAngle = p->mAngle;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::UpdateSegments()
{
    for (shared_ptr<Segment>& line : mLineSegments)
    {
        line->mPoint1->mAngle = static_cast<float>(atan2(line->mPoint1->y - mOrigin.y, line->mPoint1->x - mOrigin.x));
        line->mPoint2->mAngle = static_cast<float>(atan2(line->mPoint2->y - mOrigin.y, line->mPoint2->x - mOrigin.x));

        // Map angle between -Pi and Pi
        float dAngle = line->mPoint2->mAngle - line->mPoint1->mAngle;
        if (dAngle <= -MathHelper::Pi) { dAngle += MathHelper::TwoPi; }
        if (dAngle > MathHelper::Pi) { dAngle -= MathHelper::TwoPi; }

        line->mPoint1->mIsStart = (dAngle > 0.0f);
        line->mPoint2->mIsStart = !line->mPoint1->mIsStart;
    }
}

//////////////////////////////////////////////////////////////////////////
bool Visibility::VisibilityComputer::SegmentInFrontOf(const Segment& a, const Segment& b, const Vector2D& relativeTo)
{
    const bool a1 = MathHelper::LeftOf(*a.mPoint2, *a.mPoint1, MathHelper::Interpolate(*b.mPoint1, *b.mPoint2, 0.01f));
    const bool a2 = MathHelper::LeftOf(*a.mPoint2, *a.mPoint1, MathHelper::Interpolate(*b.mPoint2, *b.mPoint1, 0.01f));
    const bool a3 = MathHelper::LeftOf(*a.mPoint2, *a.mPoint1, relativeTo);

    const bool b1 = MathHelper::LeftOf(*b.mPoint2, *b.mPoint1, MathHelper::Interpolate(*a.mPoint1, *a.mPoint2, 0.01f));
    const bool b2 = MathHelper::LeftOf(*b.mPoint2, *b.mPoint1, MathHelper::Interpolate(*a.mPoint2, *a.mPoint1, 0.01f));
    const bool b3 = MathHelper::LeftOf(*b.mPoint2, *b.mPoint1, relativeTo);

    if (b1 == b2 && b2 != b3) return true;
    if (a1 == a2 && a2 == a3) return true;
    if (a1 == a2 && a2 != a3) return false;
    if (b1 == b2 && b2 == b3) return false;

    return false;
}

void Visibility::VisibilityComputer::AddTriangle(std::vector<Vector2D>& triangles, float angle1, float angle2, Segment* segment)
{
    const Vector2D& p1 = mOrigin;
    Vector2D p2(mOrigin.x + cos(angle1), mOrigin.y + sin(angle1));
    Vector2D p3;
    Vector2D p4;

    if (segment != nullptr)
    {
        p3.x = segment->mPoint1->x;
        p3.y = segment->mPoint1->y;

        p4.x = segment->mPoint2->x;
        p4.y = segment->mPoint2->y;
    }
    else
    {
        p3.x = mOrigin.x + cos(angle1) * mRadius * 2;
        p3.y = mOrigin.y + sin(angle1) * mRadius * 2;
        p4.x = mOrigin.x + cos(angle2) * mRadius * 2;
        p4.y = mOrigin.y + sin(angle2) * mRadius * 2;
    }

    Vector2D pBegin = MathHelper::LineLineIntersection(p3, p4, p1, p2);

    p2.x = mOrigin.x + cos(angle2);
    p2.y = mOrigin.y + sin(angle2);

    Vector2D pEnd = MathHelper::LineLineIntersection(p3, p4, p1, p2);

    triangles.emplace_back(pBegin);
    triangles.emplace_back(pEnd);
}

//////////////////////////////////////////////////////////////////////////
void Visibility::VisibilityComputer::AddSegment(const Vector2D& p1, const Vector2D& p2)
{
    shared_ptr<Segment> segment = make_shared<Segment>();
    shared_ptr<EndPoint> endPoint1 = make_shared<EndPoint>(p1, segment.get());
    shared_ptr<EndPoint> endPoint2 = make_shared<EndPoint>(p2, segment.get());

    segment->mPoint1 = endPoint1.get();
    segment->mPoint2 = endPoint2.get();

    mLineSegments.push_back(segment);
    mEndPoints.push_back(endPoint1);
    mEndPoints.push_back(endPoint2);
}
#pragma endregion

#pragma region Line

Segment::Segment(EndPoint& p1, EndPoint& p2) : mPoint1(&p1), mPoint2(&p2)
{

}

Segment::Segment() : mPoint1(), mPoint2()
{
}

Segment::Segment(const Segment& other) : mPoint1(other.mPoint1), mPoint2(other.mPoint2)
{

}

bool Segment::Equals(const Segment& a, const Segment& b)
{
    return a == b;
}


Segment& Segment::operator=(const Segment& other)
{
    if (*this != other)
    {
        mPoint1 = other.mPoint1;
        mPoint2 = other.mPoint2;
    }

    return *this;
}

bool Segment::operator==(const Segment& other) const
{
    return (mPoint1 == other.mPoint1) && (mPoint2 == other.mPoint2);
}

#pragma endregion

#pragma region LineEndPoint

//////////////////////////////////////////////////////////////////////////
Visibility::EndPoint& Visibility::EndPoint::operator=(const EndPoint& other)
{
    if (*this != other)
    {
        mAngle = other.mAngle;
        mParent = other.mParent;
        mIsStart = other.mIsStart;
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////
Visibility::EndPoint::EndPoint(const EndPoint& other) :
    Vector2D(other), mAngle(other.mAngle), mParent(other.mParent), mIsStart(other.mIsStart)
{

}

//////////////////////////////////////////////////////////////////////////
Visibility::EndPoint::EndPoint(const Vector2D& position, bool isStart, Segment* parent, float angle)
    :Vector2D(position), mAngle(angle), mParent(parent), mIsStart(isStart)
{

}

//////////////////////////////////////////////////////////////////////////
Visibility::EndPoint::EndPoint(const Vector2D& position, Segment* parent)
    : Vector2D(position), mAngle(0.f), mParent(parent), mIsStart(false)
{

}

//////////////////////////////////////////////////////////////////////////
bool Visibility::EndPoint::Equals(const EndPoint& a, const EndPoint& b)
{
    return a == b;
}

bool Visibility::EndPoint::operator<(/*const LineEndPoint& lhs,*/ const EndPoint& rhs)
{
    if (mAngle > rhs.mAngle) { return false; }
    if (mAngle < rhs.mAngle) { return true; }

    //if angles are same prefer start points
    if (!mIsStart && rhs.mIsStart) { return false; }
    if (mIsStart && !rhs.mIsStart) { return true; }

    return false;
}

//////////////////////////////////////////////////////////////////////////
bool Visibility::EndPoint::operator==(const EndPoint& other) const
{
    return (Vector2D::operator==(other)) && (mAngle == other.mAngle) && (mParent == other.mParent) && (mIsStart == other.mIsStart);
}

#pragma endregion


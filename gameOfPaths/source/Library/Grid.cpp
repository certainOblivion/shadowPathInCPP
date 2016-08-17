#include <math.h>
#include "Grid.h"
#include <algorithm>
#include <list>
#include "MathHelper.h"

using namespace Grid;

#pragma region Point

Vector2D::Vector2D(double _x, double _y) :
    x(_x), y(_y)
{}

Vector2D::Vector2D(const Vector2D& other) :
    x(other.x), y(other.y)
{}

Vector2D::Vector2D() :
    x(0), y(0)
{

}

bool Vector2D::Equals(const Vector2D& a, const Vector2D& b)
{
    return a == b;
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const
{
    return Vector2D(x + rhs.x, y + rhs.y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const
{
    return Vector2D(x - rhs.x, y - rhs.y);
}

Vector2D Vector2D::operator*(const double rhs) const
{
    return Vector2D(x * rhs, y * rhs);
}

Vector2D Vector2D::operator/(const double rhs) const
{
    Vector2D result(x / rhs, y / rhs);
    return result;
}

double Vector2D::SqrMagnitude() const
{
    return (x*x) + (y*y);
}

double Vector2D::SqrMagnitude(Vector2D a, Vector2D b)
{
    return (a - b).SqrMagnitude();
}

double Vector2D::Magnitude() const
{
    return sqrt(SqrMagnitude());
}

double Vector2D::Magnitude(Vector2D a, Vector2D b)
{
    return (a - b).Magnitude();
}

Vector2D& Vector2D::operator=(const Vector2D& other)
{
    if (*this != other)
    {
        x = other.x;
        y = other.y;
    }
    return *this;
}

bool Vector2D::operator==(const Vector2D& other) const
{
    return (x == other.x) && (y == other.y);
}

#pragma endregion

#pragma region Hex
const Hex Hex::DIRECTIONS[] = { Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1) };
const Hex Hex::DIAGONALS[] = { Hex(2, -1, -1), Hex(1, -2, 1), Hex(-1, -1, 2), Hex(-2, 1, 1), Hex(-1, 2, -1), Hex(1, 1, -2) };
Hex::Hex(int _q, int _r, int _s) :
    q(_q), r(_r), s(_s)
{}

Hex::Hex(const Hex& other) :
    q(other.q), r(other.r), s(other.s)
{}

Grid::Hex::Hex() : q(0), r(0), s(0)
{
}

Hex Hex::Add(const Hex& a, const Hex& b)
{
    return Hex(a.q + b.q, a.r + b.r, a.s + b.s);
}

Hex Hex::Subtract(const Hex& a, const Hex& b)
{
    return Hex(a.q - b.q, a.r - b.r, a.s - b.s);
}

Hex Hex::Scale(const Hex& a, int k)
{
    return Hex(a.q * k, a.r * k, a.s * k);
}

Hex Hex::Direction(int direction)
{
    return DIRECTIONS[direction];
}

Hex Hex::Neighbour(const Hex& hex, int direction)
{
    return Add(hex, Direction(direction));
}

void Hex::GetNeighbours(const Hex& hex, std::list<Hex>& neighbours)
{
    neighbours.clear();
    for (int i = 0; i < 6; i++)
    {
        neighbours.push_back(Hex::Neighbour(hex, i));
    }
}

Hex Hex::DiagonalNeighbour(const Hex& hex, int direction)
{
    return Add(hex, DIAGONALS[direction]);
}

int Hex::Length(const Hex& hex)
{
    return (int)((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

int Hex::Distance(const Hex& a, const  Hex& b)
{
    return Length(Subtract(a, b));
}

bool Hex::Equals(const Hex& a, const  Hex& b)
{
    return (a == b);
}

Hex& Hex::operator=(const Hex& other)
{
    if (*this != other)
    {
        q = other.q;
        r = other.r;
        s = other.s;
    }
    return *this;
}

bool Hex::operator==(const Hex& other)const
{
    return (q == other.q) && (r == other.r) && (s == other.s);
}

#pragma endregion

#pragma region FractionalHex	

FractionalHex::FractionalHex(double _q, double _r, double _s) :
    q(_q), r(_r), s(_s)
{

}

FractionalHex::FractionalHex(const FractionalHex& other) :
    q(other.q), r(other.r), s(other.s)
{

}

Hex FractionalHex::HexRound(const FractionalHex& h)
{
    int q = (int)(round(h.q));
    int r = (int)(round(h.r));
    int s = (int)(round(h.s));
    double q_diff = abs(q - h.q);
    double r_diff = abs(r - h.r);
    double s_diff = abs(s - h.s);
    if (q_diff > r_diff && q_diff > s_diff)
    {
        q = -r - s;
    }
    else
        if (r_diff > s_diff)
        {
            r = -q - s;
        }
        else
        {
            s = -q - r;
        }
    return Hex(q, r, s);
}

FractionalHex FractionalHex::HexLerp(const FractionalHex& a, const FractionalHex& b, double t)
{
    return FractionalHex(a.q * (1 - t) + b.q * t, a.r * (1 - t) + b.r * t, a.s * (1 - t) + b.s * t);
}

void FractionalHex::HexLineDraw(const Hex& a, const Hex& b, std::vector<Hex>& hexlist)
{
    int N = Hex::Distance(a, b);
    FractionalHex a_nudge = FractionalHex(a.q + 0.000001, a.r + 0.000001, a.s - 0.000002);
    FractionalHex b_nudge = FractionalHex(b.q + 0.000001, b.r + 0.000001, b.s - 0.000002);
    double step = 1.0 / std::max(N, 1);
    for (int i = 0; i <= N; i++)
    {
        hexlist.emplace_back(HexRound(HexLerp(a_nudge, b_nudge, step * i)));
    }
}

bool FractionalHex::Equals(const FractionalHex& a, const FractionalHex& b)
{
    return a == b;
}

bool FractionalHex::operator==(const FractionalHex& other) const
{
    return (q == other.q) && (r == other.r) && (s == other.s);
}

FractionalHex& FractionalHex::operator=(const FractionalHex& other)
{
    if (*this != other)
    {
        q = other.q;
        r = other.r;
        s = other.s;
    }

    return *this;
}

#pragma endregion

#pragma region OffsetCoord
OffsetCoord::OffsetCoord(int _col, int _row) :
    col(_col), row(_row)
{
}

OffsetCoord::OffsetCoord(const OffsetCoord& other) :
    col(other.col), row(other.row)
{
}

OffsetCoord OffsetCoord::QoffsetFromCube(int offset, const Hex& h)
{
    int col = h.q;
    int row = h.r + (int)((h.q + offset * (h.q & 1)) / 2);
    return OffsetCoord(col, row);
}

Hex OffsetCoord::QoffsetToCube(int offset, const OffsetCoord& h)
{
    int q = h.col;
    int r = h.row - (int)((h.col + offset * (h.col & 1)) / 2);
    int s = -q - r;
    return Hex(q, r, s);
}

OffsetCoord OffsetCoord::RoffsetFromCube(int offset, const Hex& h)
{
    int col = h.q + (int)((h.r + offset * (h.r & 1)) / 2);
    int row = h.r;
    return OffsetCoord(col, row);
}

Hex OffsetCoord::RoffsetToCube(int offset, const OffsetCoord& h)
{
    int q = h.col - (int)((h.row + offset * (h.row & 1)) / 2);
    int r = h.row;
    int s = -q - r;
    return Hex(q, r, s);
}

bool OffsetCoord::Equals(const OffsetCoord& a, const OffsetCoord& b)
{
    return a == b;
}

OffsetCoord& OffsetCoord::operator=(const OffsetCoord& other)
{
    if (*this != other)
    {
        col = other.col;
        row = other.row;
    }

    return *this;
}

bool OffsetCoord::operator==(const OffsetCoord& other) const
{
    return (row == other.row) && (col == other.col);
}
#pragma endregion

#pragma region Orientation
Orientation::Orientation(double _f0, double _f1, double _f2, double _f3, double _b0, double _b1, double _b2, double _b3, double _start_angle) :
    f0(_f0), f1(_f1), f2(_f2), f3(_f3), b0(_b0), b1(_b1), b2(_b2), b3(_b3), start_angle(_start_angle)
{
}

bool Orientation::Equals(const Orientation& a, const Orientation& b)
{
    return a == b;
}

Orientation& Orientation::operator=(const Orientation& other)
{
    if (*this != other)
    {
        f0 = other.f0;
        f1 = other.f1;
        f2 = other.f2;
        f3 = other.f3;
        b0 = other.b0;
        b1 = other.b1;
        b2 = other.b2;
        b3 = other.b3;
        start_angle = other.start_angle;
    }
    return *this;
}

bool Orientation::operator==(const Orientation& other) const
{
    return (f0 == other.f0) && (f1 == other.f1) && (f2 == other.f2) && (f3 == other.f3) && (b0 == other.b0) && (b1 == other.b1) && (b2 == other.b2) && (b3 == other.b3) && (start_angle == other.start_angle);
}

Orientation::Orientation(const Orientation& other) :
    f0(other.f0), f1(other.f1), f2(other.f2), f3(other.f3), b0(other.b0), b1(other.b1), b2(other.b2), b3(other.b3), start_angle(other.start_angle)
{
}
#pragma endregion

#pragma region Layout
const Orientation Layout::POINTY_TOP = Orientation(Helper::MathHelper::Sqrt3, Helper::MathHelper::Sqrt3 / 2.0, 0.0, 3.0 / 2.0, Helper::MathHelper::Sqrt3 / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);

const Orientation Layout::FLAT_TOP = Orientation(3.0 / 2.0, 0.0, Helper::MathHelper::Sqrt3 / 2.0, Helper::MathHelper::Sqrt3, 2.0 / 3.0, 0.0, -1.0 / 3.0, Helper::MathHelper::Sqrt3 / 3.0, 0.0);

Layout::Layout(const Layout& other) :
    mOrientation(other.mOrientation), mOrigin(other.mOrigin), mSize(other.mSize)
{
}

Layout::Layout(const Orientation& orientation, const Vector2D& size, const Vector2D& origin) :
    mOrientation(orientation), mSize(size), mOrigin(origin)
{
}

bool Layout::Equals(const Layout& a, const Layout& b)
{
    return a == b;
}

bool Layout::operator==(const Layout& other) const
{
    return (mOrientation == other.mOrientation) && (mOrigin == other.mOrigin) && (mSize == other.mSize);
}

Layout& Layout::operator=(const Layout& other)
{
    if (*this != other)
    {
        mOrigin = other.mOrigin;
        mOrientation = other.mOrientation;
        mSize = other.mSize;
    }

    return *this;
}

Vector2D Layout::HexToPixel(const Layout& layout, const Hex &h)
{
    const Orientation& M = layout.mOrientation;
    const Vector2D& size = layout.mSize;
    const Vector2D& origin = layout.mOrigin;
    double x = (M.f0 * h.q + M.f1 * h.r) * size.x;
    double y = (M.f2 * h.q + M.f3 * h.r) * size.y;
    return Vector2D(x + origin.x, y + origin.y);
}

FractionalHex Layout::PixelToHex(const Layout& layout, const Vector2D &p)
{
    const Orientation& M = layout.mOrientation;
    const Vector2D& size = layout.mSize;
    const Vector2D& origin = layout.mOrigin;
    const Vector2D& pt = Vector2D((p.x - origin.x) / size.x, (p.y - origin.y) / size.y);
    double q = M.b0 * pt.x + M.b1 * pt.y;
    double r = M.b2 * pt.x + M.b3 * pt.y;
    return FractionalHex(q, r, -q - r);
}

Vector2D Layout::HexCornerOffset(const Layout &layout, int corner)
{
    const Orientation& M = layout.mOrientation;
    const Vector2D& size = layout.mSize;
    double angle = 2.0 * Helper::MathHelper::Pi * (corner + M.start_angle) / 6;
    return Vector2D(size.x * cos(angle), size.y * sin(angle));
}

void Layout::PolygonCorners(const Layout& layout, const Hex &h, std::shared_ptr<std::list<Vector2D>>& corners)
{
    Vector2D center = Layout::HexToPixel(layout, h);
    for (int i = 0; i < 6; i++)
    {
        Vector2D offset = Layout::HexCornerOffset(layout, i);
        corners->emplace_back(Vector2D(center.x + offset.x, center.y + offset.y));
    }
}

bool Layout::IsOrientationPointyTop(const Layout& layout)
{
    return layout.mOrientation == POINTY_TOP;
}

bool Layout::IsOrientationFlatTop(const Layout& layout)
{
    return layout.mOrientation == FLAT_TOP;
}

#pragma endregion

#pragma region GridMesh

GridMesh::GridMesh(double mapWidth, double mapHeight, float hexSize/*= 1.f*/) :
    mPointWidth(hexSize/*/worldToGridRatio*/),
    mLayout(Layout(Layout::POINTY_TOP, Vector2D(mPointWidth, mPointWidth), Vector2D(-(mapWidth / 2), -(mapHeight / 2))))
{
}

void GridMesh::CreateMap(double mapWidth, double mapHeight, std::unordered_set<Hex>& map)
{
    for (int r = 0; r < mapHeight; r++)
    {
        int r_offset = r >> 1;
        for (int q = -r_offset; q < mapWidth - r_offset; q++)
        {
            map.emplace(Hex(q, r, -q - r));
        }
    }
}

bool GridMesh::Equals(const GridMesh& a, const GridMesh& b)
{
    return a == b;
}

const Layout& GridMesh::GetLayout() const
{
    return mLayout;
}

GridMesh& GridMesh::operator=(const GridMesh& other)
{
    if (*this != other)
    {
        mPointWidth = other.mPointWidth;
        mLayout = other.mLayout;
    }
    return *this;
}

bool GridMesh::operator==(const GridMesh& other) const
{
    return (mPointWidth == other.mPointWidth) && (mLayout == other.mLayout);
}

GridMesh::GridMesh(const GridMesh& other) :
    mPointWidth(other.mPointWidth), mLayout(other.mLayout)
{
}

#pragma endregion


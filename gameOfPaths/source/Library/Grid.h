#pragma once
#include <list>
#include <unordered_set>
#include <memory>

#pragma region Point
class Vector2D
{
public:
    Vector2D();
    Vector2D(double _x, double _y);
    Vector2D(const Vector2D& other);
    static bool Equals(const Vector2D& a, const Vector2D& b);
    bool operator==(const Vector2D& other)const;
    bool operator!=(const Vector2D& other)const { return !(operator==(other)); }
    Vector2D& operator=(const Vector2D& other);
    Vector2D operator+(const Vector2D& rhs) const;
    Vector2D operator-(const Vector2D& rhs) const;
    Vector2D operator*(const double rhs) const;
    Vector2D operator/(const double rhs) const;
    double SqrMagnitude() const;
    double Magnitude() const;
    static double SqrMagnitude(Vector2D a, Vector2D b);
    static double Magnitude(Vector2D a, Vector2D b);
    double x;
    double y;
};
#pragma endregion

namespace Grid
{

#pragma region Hex
    class Hex
    {
        friend struct std::hash<Hex>;
        friend class FractionalHex;
        friend class OffsetCoord;
        friend class Layout;
    public:
        Hex(int _q, int _r, int _s);
        Hex(const Hex& other);
        static Hex Add(const Hex& a, const Hex& b);
        static Hex Subtract(const Hex& a, const Hex& b);
        static Hex Scale(const Hex& a, int k);
        static Hex Direction(int direction);
        static Hex Neighbour(const Hex& hex, int direction);
        static void GetNeighbours(const Hex& hex, std::list<Hex>& neighbours);
        static Hex DiagonalNeighbour(const Hex& hex, int direction);
        static int Length(const Hex& hex);
        static int Distance(const Hex& a, const  Hex& b);
        static bool Equals(const Hex& a, const Hex& b);
        bool operator==(const Hex& other)const;
        bool operator!=(const Hex& other)const { return !(operator==(other)); }
        Hex& operator=(const Hex& other);
        static const Hex DIRECTIONS[];
        static const Hex DIAGONALS[];
        Hex();
        int q;
        int r;
        int s;
    };
#pragma endregion



#pragma region FractionalHex	
    class FractionalHex
    {
    public:
        FractionalHex(double _q, double _r, double _s);
        FractionalHex(const FractionalHex& other);
        static Hex HexRound(const FractionalHex& h);
        static FractionalHex HexLerp(const FractionalHex& a, const FractionalHex& b, double t);
        static void FractionalHex::HexLineDraw(const Hex& a, const Hex& b, std::vector<Hex>& hexlist);
        FractionalHex& operator=(const FractionalHex& other);
        static bool Equals(const FractionalHex& a, const FractionalHex& b);
        bool operator==(const FractionalHex& other)const;
        bool operator!=(const FractionalHex& other)const { return !(operator==(other)); }
    private:
        FractionalHex() = delete;

        double q;
        double r;
        double s;
    };


#pragma endregion

#pragma region OffsetCoord
    class OffsetCoord
    {
    public:
        OffsetCoord(int _col, int _row);
        OffsetCoord(const OffsetCoord& other);
        static OffsetCoord QoffsetFromCube(int offset, const Hex& h);
        static Hex QoffsetToCube(int offset, const OffsetCoord& h);
        static OffsetCoord RoffsetFromCube(int offset, const Hex& h);
        static Hex RoffsetToCube(int offset, const OffsetCoord& h);
        static bool Equals(const OffsetCoord& a, const OffsetCoord& b);
        bool operator==(const OffsetCoord& other)const;
        bool operator!=(const OffsetCoord& other)const { return !(operator==(other)); }
        OffsetCoord& operator=(const OffsetCoord& other);
    private:
        OffsetCoord() = delete;

        int col;
        int row;
    };

#pragma endregion

#pragma region Orientation
    class Orientation
    {
        friend class Layout;
    public:
        Orientation(double _f0, double _f1, double _f2, double _f3, double _b0, double _b1, double _b2, double _b3, double _start_angle);
        Orientation(const Orientation& other);

        static bool Equals(const Orientation& a, const Orientation& b);
        bool operator==(const Orientation& other)const;
        bool operator!=(const Orientation& other)const { return !(operator==(other)); }
        Orientation& operator=(const Orientation& other);

    private:
        Orientation() = delete;
        double f0, f1, f2, f3, b0, b1, b2, b3, start_angle;
    };
#pragma endregion

#pragma region Layout
    class Layout
    {
    public:
        enum POINTYTOP_CORNERS
        {
            POINTYTOP_CORNERS_RIGHTUP,
            POINTYTOP_CORNERS_UP,
            POINTYTOP_CORNERS_LEFTUP,
            POINTYTOP_CORNERS_LEFTDOWN,
            POINTYTOP_CORNERS_DOWN,
            POINTYTOP_CORNERS_RIGHTDOWN
        };

        enum FLATTOP_CORNERS
        {
            FLATTOP_CORNERS_RIGHT,
            FLATTOP_CORNERS_RIGHTUP,
            FLATTOP_CORNERS_LEFTUP,
            FLATTOP_CORNERS_LEFT,
            FLATTOP_CORNERS_LEFTDOWN,
            FLATTOP_CORNERS_RIGHTDOWN
        };

        Layout(const Orientation& orientation, const Vector2D& size, const Vector2D& origin);
        Layout(const Layout& other);

        static bool Equals(const Layout& a, const Layout& b);
        bool operator==(const Layout& other)const;
        bool operator!=(const Layout& other)const { return !(operator==(other)); }
        Layout& operator=(const Layout& other);
        static Vector2D HexToPixel(const Layout& layout, const Hex &h);
        static FractionalHex PixelToHex(const Layout& layout, const Vector2D &p);
        static Vector2D HexCornerOffset(const Layout &layout, int corner);
        static void PolygonCorners(const Layout& layout, const Hex &h, std::shared_ptr<std::list<Vector2D>>& corners);
        static bool IsOrientationPointyTop(const Layout& layout);
        static bool IsOrientationFlatTop(const Layout& layout);
        static const Orientation POINTY_TOP;
        static const Orientation FLAT_TOP;

    private:
        Layout() = delete;
        Orientation mOrientation;
        Vector2D mSize;
        Vector2D mOrigin;

    };
#pragma endregion

#pragma region GridMesh
    class GridMesh
    {
    public:
        GridMesh(double mapWidth, double mapHeight, float hexSize = 1.f);
        GridMesh(const GridMesh& other);

        static void CreateMap(double mapWidth, double mapHeight, std::unordered_set<Hex>& createdHex);
        static bool Equals(const GridMesh& a, const GridMesh& b);
        bool operator==(const GridMesh& other)const;
        bool operator!=(const GridMesh& other)const { return !(operator==(other)); }
        GridMesh& operator=(const GridMesh& other);
        const Layout& GetLayout() const;
    private:
        float mPointWidth;
        Layout mLayout;
    };
#pragma endregion


}

#pragma region Hasher
namespace std
{
    template<>
    struct hash<Grid::Hex>
    {
    public:
        std::size_t operator()(const Grid::Hex& h) const
        {
            using std::size_t;
            using std::hash;

            return (hash<int>()(h.q)
                ^ (hash<int>()(h.r) << 1))
                ^ (hash<int>()(h.s) << 2);
        }
    };

    template<>
    struct hash<Vector2D>
    {
    public:
        std::size_t operator()(const Vector2D& p) const
        {
            using std::size_t;
            using std::hash;

            return ((hash<double>()(p.x)
                ^ (hash<double>()(p.y) << 1)));
        }
    };
}
#pragma endregion
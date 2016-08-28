#include "GridHelper.h"
#include "Grid.h"
#include "MathHelper.h"
#include <algorithm>
#include "AssertCheck.h"

using namespace Grid;
using namespace Helper;

void Helper::GridHelper::RectangleToHexList(const Vector2D& center, const Vector2D& dimensions, float rotationInRadians, const Grid::GridMesh& grid, std::vector<Grid::Hex>& hexList)
{
    hexList.clear();
    std::vector<Vector2D> corners;
    Helper::MathHelper::GetRectangleCorners(center, dimensions, rotationInRadians, corners);

     std::vector<Hex> topHexes;
     std::vector<Hex> bottomHexes;
 
     LineToHexList(corners[0], corners[1], grid, topHexes);
     LineToHexList(corners[3], corners[2], grid, bottomHexes);
 
     for (size_t topHexIndex = 0; topHexIndex < topHexes.size(); topHexIndex++)
     {
         const int bottomHexIndex = std::min(topHexIndex, bottomHexes.size() - 1);
         FractionalHex::HexLineDraw(topHexes[topHexIndex], bottomHexes[bottomHexIndex], hexList);
     }
}

void Helper::GridHelper::LineToHexList(const Vector2D& endpoint1, const Vector2D& endpoint2, const Grid::GridMesh& grid, std::vector<Grid::Hex>& hexList)
{
    hexList.clear();

    Hex a = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), endpoint1));
    Hex b = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), endpoint2));
    FractionalHex::HexLineDraw(a, b, hexList);
}

Grid::Hex Helper::GridHelper::PixelToHex(const Vector2D& pixel, const Grid::Layout& layout)
{
    return FractionalHex::HexRound(Layout::PixelToHex(layout, pixel));
}

Vector2D Helper::GridHelper::HexToPixel(const Hex& hex, const Grid::Layout& layout)
{
    return Layout::HexToPixel(layout, hex);
}

void Helper::GridHelper::ConvexAxisAlignedShapeToHexList(const std::vector<Vector2D>& vertexList, const Grid::GridMesh& grid, std::vector<Grid::Hex>& hexList)
{
    throw_assert(vertexList.size() > 2, "there needs to be at least 3 verteces to make a shape");
    int q_min, q_max, r_min, r_max, s_min, s_max;

    Hex a = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), vertexList[0]));

    q_min = q_max = a.q;
    r_min = r_max = a.r;
    s_min = s_max = a.s;

    for (size_t i = 1; i < vertexList.size(); i++)
    {
        a = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), vertexList[i]));

        q_min = std::min(q_min, a.q);
        q_max = std::max(q_max, a.q);
        r_min = std::min(r_min, a.r);
        r_max = std::max(r_max, a.r);
        s_min = std::min(s_min, a.s);
        s_max = std::max(s_max, a.s);
    }

    for (int r = r_min; r <= r_max; r++)
    {
        for (int q = q_min; q <= q_max; q++)
        {
            int s = -q - r;
            if (s >= s_min && s <= s_max)
            {
                hexList.push_back(Hex(q, r, -q - r));
            }
        }
    }
}

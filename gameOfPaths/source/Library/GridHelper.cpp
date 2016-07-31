#include "GridHelper.h"
#include "Grid.h"
#include "MathHelper.h"
#include <algorithm>

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

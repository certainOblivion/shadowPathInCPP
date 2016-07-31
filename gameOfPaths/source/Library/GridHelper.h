#pragma once
#include <memory>
#include <list>
#include "Grid.h"
namespace Helper
{
    class GridHelper
    {
    public:
        static Grid::Hex PixelToHex(const Vector2D& pixel, const Grid::Layout& layout);
        static void RectangleToHexList(const Vector2D& center, const Vector2D& dimensions, float rotationInRadians, const Grid::GridMesh& grid, std::vector<Grid::Hex>& hexList);
        static void LineToHexList(const Vector2D& endpoint1, const Vector2D& endpoint2, const Grid::GridMesh& grid, std::vector<Grid::Hex>& hexList);
        static Vector2D HexToPixel(const Grid::Hex& pixel, const Grid::Layout& layout);
    };
}
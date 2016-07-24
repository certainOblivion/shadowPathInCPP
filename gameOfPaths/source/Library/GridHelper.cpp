#include "GridHelper.h"
#include "Grid.h"
#include "MathHelper.h"

using namespace Grid;
using namespace Helper;

void Helper::GridHelper::RectangleToHexList(const Vector2D& center, const Vector2D& dimensions, float rotationInRadians, const Grid::GridMesh& grid, std::list<Grid::Hex>& hexList)
{
	hexList.clear();
	std::vector<Vector2D> corners;
	Helper::MathHelper::GetRectangleCorners(center, dimensions, rotationInRadians, corners);

	std::list<Hex> hexes;

	auto addLineHexToList = [&](const Vector2D& p1, const Vector2D& p2)
	{
		LineToHexList(p1, p2, grid, hexes);
		for (Hex& h : hexes)
		{
			hexList.emplace_back(h);
		}
		hexes.clear();
	};

	addLineHexToList(corners[0], corners[1]);
	addLineHexToList(corners[1], corners[2]);
	addLineHexToList(corners[2], corners[3]);
	addLineHexToList(corners[3], corners[0]);
}

void Helper::GridHelper::LineToHexList(const Vector2D& endpoint1, const Vector2D& endpoint2, const Grid::GridMesh& grid, std::list<Grid::Hex>& hexList)
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

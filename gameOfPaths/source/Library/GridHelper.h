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
		static void GetHexOnVerticalLine(const Vector2D& top, const Vector2D& bottom,const Grid::GridMesh& grid, std::shared_ptr<std::list<Grid::Hex>>& hexList);
		static void GetHexOnHorizontalLine(const Vector2D& left, const Vector2D& right, const Grid::GridMesh& grid, std::shared_ptr<std::list<Grid::Hex>>& hexList);
		static void RectangleToHexList(const Vector2D& center, const Vector2D& dimensions, const Grid::GridMesh& grid, std::shared_ptr<std::unordered_set<Grid::Hex>>& hexList);
	};
}
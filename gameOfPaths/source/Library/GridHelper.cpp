#include "GridHelper.h"
#include "Grid.h"

using namespace Grid;
using namespace Helper;

void GridHelper::GetHexOnVerticalLine(const Vector2D& top, const Vector2D& bottom, const GridMesh& grid, std::shared_ptr<std::list<Grid::Hex>>& hexList)
{
	hexList->clear();
	Hex hexTop = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), top));
	Hex hexBottom = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), bottom));

	hexList->emplace_back(hexTop);
	hexList->emplace_back(hexBottom);
	const int bottomPoint = Layout::IsOrientationPointyTop(grid.GetLayout()) ? Layout::POINTYTOP_CORNERS::POINTYTOP_CORNERS_DOWN : Layout::FLATTOP_CORNERS::FLATTOP_CORNERS_LEFTDOWN;
	//starting from the top and getting all the hexes till the bottom
	Hex currentHex = hexTop;
	Vector2D currentPoint = top;
	double nudge = 0.0001;
	while (currentHex!=hexBottom)
	{
		//get bottom point
		Vector2D bottomCorner = Layout::HexCornerOffset(grid.GetLayout(), bottomPoint);
		Vector2D center = Layout::HexToPixel(grid.GetLayout(), currentHex);
		currentPoint.y = (float)(bottomCorner.y + center.y - nudge);
		if (currentPoint.y < bottom.y)
		{
			break;
		}
		currentHex = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), currentPoint));
		hexList->emplace_back(currentHex);
	}
}

void GridHelper::GetHexOnHorizontalLine(const Vector2D& left, const Vector2D& right, const Grid::GridMesh& grid, std::shared_ptr<std::list<Grid::Hex>>& hexList)
{
	hexList->clear();
	Hex hexLeft = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), left));
	Hex hexRight = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), right));

	hexList->emplace_back(hexRight);
	hexList->emplace_back(hexLeft);

	//starting from the top and getting all the hexes till the bottom
	Hex currentHex = hexRight;
	Vector2D currentPoint = right;
	double nudge = 0.0001;
	const int leftmostPoint = Layout::IsOrientationPointyTop(grid.GetLayout()) ? Layout::POINTYTOP_CORNERS::POINTYTOP_CORNERS_LEFTUP : Layout::FLATTOP_CORNERS::FLATTOP_CORNERS_LEFT;
	while (currentHex!= hexLeft)
	{
		//get bottom point
		Vector2D leastXCorner = Layout::HexCornerOffset(grid.GetLayout(), leftmostPoint);
		Vector2D center = Layout::HexToPixel(grid.GetLayout(), currentHex);
		currentPoint.x = (float)(leastXCorner.x + center.x - nudge);
		if (currentPoint.x < left.x)
		{
			break;
		}
		currentHex = FractionalHex::HexRound(Layout::PixelToHex(grid.GetLayout(), currentPoint));
		hexList->emplace_back(currentHex);
	}
}

void GridHelper::RectangleToHexList(const Vector2D& center, const Vector2D& dimensions, const Grid::GridMesh& grid, std::shared_ptr<std::unordered_set<Grid::Hex>>& hexList)
{
	hexList->clear();

	Vector2D boxTopLeft((center.x - dimensions.x / 2), (center.y + dimensions.y / 2));
	Vector2D boxBottomLeft(center - (dimensions / 2));
	Vector2D boxBottomRight((center.x + dimensions.x / 2), (center.y - dimensions.y / 2));
	Vector2D boxTopRight(center + (dimensions / 2));

	std::shared_ptr<std::list<Hex>> hexes = std::make_shared<std::list<Hex>>();
	auto addHexesToHexList = [&]() {
		for (Hex& h : *hexes)
		{
			hexList->emplace(h);
		}
	};
	GetHexOnVerticalLine(boxTopLeft, boxBottomLeft, grid,hexes);
	addHexesToHexList();
 
	GetHexOnVerticalLine(boxTopRight, boxBottomRight, grid, hexes);
	addHexesToHexList();

	GetHexOnHorizontalLine(boxTopRight, boxTopLeft, grid, hexes);
	addHexesToHexList();

	GetHexOnHorizontalLine(boxBottomLeft, boxBottomRight, grid, hexes);
	addHexesToHexList();
}

Grid::Hex Helper::GridHelper::PixelToHex(const Vector2D& pixel, const Grid::Layout& layout)
{
	return FractionalHex::HexRound(Layout::PixelToHex(layout, pixel));
}

#include "Pathfinding.h"
#if !RELEASE
#include <chrono>
#endif
#include <queue>
#include "GridHelper.h"

using namespace PathSystem;
using namespace Grid;
using namespace std;
using namespace Helper;

Pathfinding::~Pathfinding()
{
}

PathSystem::Pathfinding::Pathfinding(int mapWidth, int mapHeight, float hexSize)
	: GridWidth ( mapWidth), GridHeight (mapHeight), mGridMesh(GridMesh(mapWidth, mapHeight,hexSize))
{
}

void PathSystem::Pathfinding::AddObstacle(const Vector2D& center, const Vector2D& dimensions, float rotation)
{
	std::list<Hex> hexList;
	GridHelper::RectangleToHexList(center, dimensions, rotation, mGridMesh, hexList);
	for (const Hex& h : hexList)
	{
		mBlockedHex.emplace(h);
	}
}


int PathSystem::Pathfinding::CalculateHeuristic(Hex a, Hex b)
{
	Vector2D pixelA = Layout::HexToPixel(mGridMesh.GetLayout(), a);
	Vector2D pixelB = Layout::HexToPixel(mGridMesh.GetLayout(), b);

	return /*static_cast<int>((pixelA - pixelB).SqrMagnitude() * 10) +*/ Hex::Distance(a, b) /** 100*/;
}


void PathSystem::Pathfinding::SmoothPath(std::list<Vector2D> &path)
{
	if (path.size() < 3)
	{
		return;
	}
	auto& source = path.begin();
	auto& nodeToDelete = std::next(source, 1);

	while (std::next(nodeToDelete, 1) != path.end())
	{
		if (IsVisible(*source, *std::next(nodeToDelete, 1)))
		{
			path.erase(nodeToDelete);//nodeToDelete is invalid now
			nodeToDelete = std::next(source, 1);
		}
		else
		{
			source = nodeToDelete;
			nodeToDelete = std::next(source, 1);
		}
	}
}

bool PathSystem::Pathfinding::IsVisible(Vector2D observer, Vector2D destination) const
{
	std::list<Hex> hexesInBetween;

	FractionalHex::HexLineDraw(PixelToHex(observer), PixelToHex(destination), hexesInBetween);

	for (Hex& hex : hexesInBetween)
	{
		if (mBlockedHex.count(hex))
		{
			return false;
		}
	}

	return true;
}


Grid::Hex PathSystem::Pathfinding::PixelToHex(const Vector2D& pixel) const
{
	return GridHelper::PixelToHex(pixel, mGridMesh.GetLayout());
}


Vector2D PathSystem::Pathfinding::HexToPixel(const Grid::Hex& hex) const
{
	return Layout::HexToPixel(mGridMesh.GetLayout(), hex);
}

#if !RELEASE
const Grid::Layout& PathSystem::Pathfinding::GetLayout() const
{
	return mGridMesh.GetLayout();
}

void PathSystem::Pathfinding::GetBlockedHex(std::unordered_set<Grid::Hex>& hexSet) const
{
	hexSet = mBlockedHex;
}

void PathSystem::Pathfinding::GetAllHex(std::unordered_set<Grid::Hex>& hexSet) const
{
	hexSet = mAllHex;
}

#endif

 #if RELEASE
void PathSystem::Pathfinding::GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path)
 {
#else
 void PathSystem::Pathfinding::GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path, std::list<Grid::Hex>& hexInPath, std::unordered_set<Grid::Hex>& testedHex, long& timeToFindPath)
 {
	 const auto startTime = std::chrono::high_resolution_clock::now();;
	 timeToFindPath = 0;
#endif
	 Hex startHex = PixelToHex(start);
	 Hex destinationHex = PixelToHex(destination);

	 std::unordered_map<Hex, Hex> came_from;
#if !RELEASE
	 testedHex.clear();
#endif

	 bool pathFound = false;
	 if (!mBlockedHex.count(destinationHex) && !mBlockedHex.count(startHex))
	 {
		 std::priority_queue<HexPriorityNode, std::vector<HexPriorityNode>> frontier;
		 HexPriorityNode startNode(startHex, 0.f);
		 frontier.emplace(startNode);
		 std::unordered_map<Hex, int> cost_so_far;

		 came_from[startHex] = startHex;
		 cost_so_far[startHex] = 0;

		 while (frontier.size() > 0)
		 {
			 HexPriorityNode current = frontier.top();
			 frontier.pop();
#if !RELEASE
			 testedHex.emplace(current.mHex);
#endif
			 if (current.mHex == destinationHex)
			 {
#if !RELEASE
				 auto finishTime = std::chrono::high_resolution_clock::now();
				 timeToFindPath = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - startTime).count());
#endif
				 pathFound = true;
				 break;
			 }

			 std::list<Hex> neighbours;
			 Hex::GetNeighbours(current.mHex, neighbours);
			 for (Hex& next : neighbours)
			 {
				 if (mBlockedHex.count(next))
				 {
					 continue;
				 }

				 int new_cost = cost_so_far[current.mHex] + UNBLOCKED_HEX_COST;

				 if ((cost_so_far.count(next) == 0) || (new_cost < cost_so_far[next]))
				 {
					 cost_so_far[next] = new_cost;
					 int priority = new_cost + CalculateHeuristic(destinationHex, next);
					 HexPriorityNode nextNode(next,static_cast<float>( priority));
					 frontier.push(nextNode);
					 came_from[next] = current.mHex;
				 }
			 }
		 }
	 }

	 if (pathFound && came_from.size() > 0)
	 {
#if !RELEASE
		 hexInPath.clear();
#endif
		 Hex currentHex = destinationHex;

		 while (came_from[currentHex] != startHex)
		 {
#if !RELEASE
			 hexInPath.emplace_back(currentHex);
#endif

			 path.emplace_front(HexToPixel(currentHex));
			 currentHex = came_from[currentHex];

		 }

		 path.emplace_front(start);
		 path.emplace_back(destination);
		 SmoothPath(path);
	 }

 }



#include "pch.h"
#include "Pathfinding.h"
#include "GridHelper.h"
#if !RELEASE
#include <chrono>
#endif
#include <queue>

using namespace PathSystem;
using namespace Grid;
using namespace std;
using namespace Helper;

Pathfinding::~Pathfinding()
{
}

PathSystem::Pathfinding::Pathfinding(int mapWidth, int mapHeight, float hexDiminishFactor, float worldToGridRatio, std::list<PathObstacle>& obstacles, std::list<PathActor>& actors)
	: GridWidth ( mapWidth), GridHeight (mapHeight), mGridMesh(GridMesh(mapWidth, mapHeight,hexDiminishFactor, worldToGridRatio))
{
	if (obstacles.size() > 0)
	{
		for (const PathObstacle& obstacle : obstacles)
		{
			AddObstacle(obstacle.Position(), obstacle.Dimensions());
		}
	}

	if (actors.size() > 0)
	{
		for (PathActor obstacle : actors)
		{

		}
	}
}

void PathSystem::Pathfinding::AddObstacle(const Vector2D& center, const Vector2D& dimensions)
{
	std::shared_ptr<std::unordered_set<Hex>> hexList = std::make_shared<std::unordered_set<Hex>>();
	GridHelper::RectangleToHexList(center, dimensions, mGridMesh, hexList);
	for (const Hex& h : *hexList)
	{
		mBlockedHex.emplace(h);
	}
}


int PathSystem::Pathfinding::CalculateHeuristic(Hex a, Hex b)
{
	return 0;
}


void PathSystem::Pathfinding::SmoothPath(std::list<Vector2D> &path)
{
	if (path.size() < 3)
	{
		return;
	}
	auto& source = path.begin();
	auto& nodeToDelete = ++source;

	while ((++nodeToDelete) != path.end())
	{
		if (IsVisible(*source, *(++nodeToDelete)))
		{
			auto& temp = nodeToDelete;
			nodeToDelete = ++nodeToDelete;
			path.remove(*temp);
		}
		else
		{
			source = nodeToDelete;
			nodeToDelete = ++source;
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


 #if RELEASE
void PathSystem::Pathfinding::GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path)
 {
#else
 void PathSystem::Pathfinding::GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path, std::list<Grid::Hex>& hexInPath, std::list<Grid::Hex>& testedHex, long& timeToFindPath)
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
		 std::priority_queue<HexPriorityNode, std::vector<HexPriorityNode>, CompareHexNode> frontier;
		 HexPriorityNode startNode(startHex, 0.f);
		 frontier.emplace(startNode);
		 std::unordered_map<Hex, int> cost_so_far;

		 came_from.emplace(startHex,startHex); // to use the [] operator in map, the type needs to have a parameterless constructor, working around it
		 cost_so_far[startHex] = 0;

		 while (frontier.size() > 0)
		 {
			 HexPriorityNode current = frontier.top();
			 frontier.pop();
#if !RELEASE
			 testedHex.emplace_back(current.mHex);
#endif
			 if (current.mHex == destinationHex)
			 {
#if !RELEASE
				 auto finishTime = std::chrono::high_resolution_clock::now();
				 timeToFindPath = static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count());
#endif
				 pathFound = true;
				 break;
			 }

			 for (int i = 0; i < HEX_NUM_SIDES; i++)
			 {
				 Hex next = Hex::Neighbour(current.mHex, i);

				 if (mBlockedHex.count(next))
				 {
					 continue;
				 }

				 int new_cost = cost_so_far[current.mHex] + UNBLOCKED_HEX_COST;

				 if ((cost_so_far.find(next) == cost_so_far.end()) || new_cost < cost_so_far[next])
				 {
					 cost_so_far[next] = new_cost;
					 int priority = new_cost + CalculateHeuristic(destinationHex, next);
					 HexPriorityNode nextNode(next,static_cast<float>( priority));
					 frontier.emplace(nextNode);
					 came_from.emplace(next, current.mHex);
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

		 while (came_from.find(currentHex)->second != destinationHex)
		 {
#if !RELEASE
			 hexInPath.emplace_back(currentHex);
#endif

			 path.emplace_front(HexToPixel(currentHex));
			 currentHex = came_from.find(currentHex)->second;
		 }

		 path.emplace_front(start);
		 SmoothPath(path);
	 }

 }



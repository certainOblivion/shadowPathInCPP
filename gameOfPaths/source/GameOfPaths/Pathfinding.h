#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include "PathObstacle.h"
#include "PathActor.h"
#include "Rect.h"
namespace PathSystem
{
	class Pathfinding
	{
	public:
		Pathfinding(int mapWidth, int mapHeight, float hexDiminishFactor, float worldToGridRatio, std::list<PathObstacle>& obstacles, std::list<PathActor>& actors);
		Pathfinding() = delete;
		~Pathfinding();
		void AddObstacle(const Vector2D& center, const Vector2D& dimensions);
#if RELEASE
		void GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path);
#else
		void GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path, std::list<Grid::Hex>& hexInPath, std::list<Grid::Hex>& testedHex, long& timeToFindPath);
#endif
		bool IsVisible(Vector2D observer, Vector2D destination) const;
		Grid::Hex PixelToHex(const Vector2D& pixel) const;
		Vector2D HexToPixel(const Grid::Hex& hex) const;

	private:
		int CalculateHeuristic(Grid::Hex a, Grid::Hex b);
		void SmoothPath(std::list<Vector2D> &path);
		int GridWidth;
		int GridHeight;
		int GridSize;

		const int HEX_NUM_SIDES = 6;
		const int UNBLOCKED_HEX_COST = 1;
		const int SPATIAL_HASH_CELL_SIZE = 5;

		Grid::GridMesh mGridMesh;
		std::unordered_set<Grid::Hex> mBlockedHex;
		std::list<Rect> mObstacles;
		std::unordered_map<PathActor, Vector2D> ActorData;
	};

	struct HexPriorityNode
	{
		Grid::Hex& mHex;
		float mPriority;

		HexPriorityNode(Grid::Hex& hex, float priority) : mHex(hex), mPriority(priority) {}
 		HexPriorityNode& operator=(const HexPriorityNode& other)
 		{
 			mHex = other.mHex;
 			mPriority = other.mPriority;

			return *this;
 		}
	};

 	struct CompareHexNode
 	{
 		bool operator()(const HexPriorityNode& lhs, const HexPriorityNode& rhs) const
 		{
 			return lhs.mPriority < rhs.mPriority;
 		}
 	};
}


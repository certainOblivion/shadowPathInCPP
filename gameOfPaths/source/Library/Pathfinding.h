#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include "Rect.h"
namespace PathSystem
{
	class Pathfinding
	{
	public:
		Pathfinding(int mapWidth, int mapHeight, float hexSize);
		Pathfinding() = delete;
		~Pathfinding();
		void AddObstacle(const Vector2D& center, const Vector2D& dimensions, float rotation);
#if RELEASE
		void GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path);
#else
		void GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path, std::list<Grid::Hex>& hexInPath, std::unordered_set<Grid::Hex>& testedHex, long& timeToFindPath);
		const Grid::Layout& GetLayout() const;
		void GetBlockedHex(std::unordered_set<Grid::Hex>& hexSet) const;
		void GetAllHex(std::unordered_set<Grid::Hex>& hexSet) const;
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
#if !RELEASE
		std::unordered_set<Grid::Hex> mAllHex;
#endif
	};

	struct HexPriorityNode
	{
		Grid::Hex mHex;
		float mPriority;

		HexPriorityNode(const Grid::Hex& hex, float priority) : mHex(hex), mPriority(priority) {}
 		HexPriorityNode& operator=(const HexPriorityNode& other)
 		{
 			mHex = other.mHex;
 			mPriority = other.mPriority;

			return *this;
 		}
	};
}

namespace std
{
	template<>
	struct less<PathSystem::HexPriorityNode>
	{
	public:
		bool operator()(const PathSystem::HexPriorityNode& lhs, const PathSystem::HexPriorityNode& rhs) const
		{
			return lhs.mPriority > rhs.mPriority;
		}
	};
}


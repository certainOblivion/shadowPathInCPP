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
        void AddBlockedLine(const Vector2D&p1, const Vector2D&p2);
        void GetUnblockedPathPointsInRect(const Vector2D& rectCenter, const Vector2D& rectDimension, float rectRotation, std::vector<Vector2D>& unblockedPos);

        /** Updates the path to the target assuming that the target has moved from the original place
        * Algorithm assumes the delta between the last position and the current position is small (locomotion not teleportation)
        * @param path calculated to the target's original position
        * @param target's new position
        */
        void UpdatePathToMovingTarget(std::list<Vector2D> &path, const Vector2D& newTarget, const Vector2D& currentPosition);
        
        /** Checks whether there's a line of sight between the two points
        * @param position of observer
        * @param position of target
        */
        bool IsVisible(Vector2D observer, Vector2D destination) const;
        Grid::Hex PixelToHex(const Vector2D& pixel) const;
        Vector2D HexToPixel(const Grid::Hex& hex) const;

#if RELEASE
        void GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path);
#else
        void GetBlockedHexList(std::unordered_set<Grid::Hex>& hexSet) const;
        void GetPath(const Vector2D& start, const Vector2D &destination, std::list<Vector2D> &path, std::list<Grid::Hex>* hexInPath = nullptr, std::unordered_set<Grid::Hex>* testedHex = nullptr, long* timeToFindPath = nullptr);
        const Grid::Layout& GetLayout() const;
#endif

    private:
        int CalculateHeuristic(Grid::Hex a, Grid::Hex b);
        void SmoothPath(std::list<Vector2D> &path);
        int GridWidth;
        int GridHeight;
        int GridSize;

        const int UNBLOCKED_HEX_COST = 1;

        Grid::GridMesh mGridMesh;
        std::unordered_set<Grid::Hex> mBlockedHex;
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


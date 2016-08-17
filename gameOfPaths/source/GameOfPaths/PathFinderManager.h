#pragma once
#include <unordered_map>
#include "Library\Grid.h"
#include <functional>
#include "Library\Pathfinding.h"
#include <mutex>
#include <condition_variable>

class Request
{
public:
    Request(std::function<void(std::list<Vector2D>, bool)> callback);
    void NotifyCallback(std::list<Vector2D> path, bool pathfound);
    virtual bool ProcessRequest(const PathSystem::Pathfinding* pathfinder) = 0;
private:
    std::function<void(std::list<Vector2D>, bool)> mCallback;
};

class PathFinderManager 
{
    class PathRequest : public Request
    {
        friend class PathFinderManager;
        Vector2D mPathStart;
        Vector2D mPathEnd;

    public:
        PathRequest(Vector2D start, Vector2D end, std::function<void(std::list<Vector2D>, bool)> callback);
        virtual bool ProcessRequest(const PathSystem::Pathfinding* pathfinder) override;

    };

    class PathSmoothRequest : public Request
    {
        friend class PathFinderManager;
        std::list<Vector2D> mPath;
    public:
        PathSmoothRequest(std::list<Vector2D> path, std::function<void(std::list<Vector2D>, bool)> callback);
        virtual bool ProcessRequest(const PathSystem::Pathfinding* pathfinder) override;

    };

public:
    ~PathFinderManager();
    static void Init(int mapWidth, int mapHeight, float hexSize);
    static PathFinderManager& GetInstance();
    static void RequestPath(Vector2D start, Vector2D end, std::function<void(std::list<Vector2D>, bool)> callback);
    static void RequestPathSmoothing(std::list<Vector2D> path, std::function<void(std::list<Vector2D>, bool)> callback);
    static void AddObstacle(Vector2D position, Vector2D dimension, float rotation, bool isVisible /*= true*/, bool isHollow /*= false*/);
    static void Start();
    static void Stop();

#if !RELEASE
    static Grid::Layout GetLayout();
#endif
private:
    PathFinderManager();
    std::shared_ptr<PathSystem::Pathfinding> mPathfinder;
    bool mIsProcessRunning;
    std::list<std::shared_ptr<Request>> mRequestQueue;
    std::shared_ptr<Request> mCurrentRequest;
    std::recursive_mutex mMutex;
    std::condition_variable_any mConditionVar;
    void TryProcessNext();
    void FinishedProcessingPath(std::list<Vector2D>& path, bool foundPath) const;
};


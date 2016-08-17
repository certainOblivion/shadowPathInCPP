#include "pch.h"
#include "PathFinderManager.h"
#include "Library\MathHelper.h"
#include <thread>
#include <chrono>


PathFinderManager::PathFinderManager() : mCurrentRequest(nullptr), mIsProcessRunning(false)
{
}

void PathFinderManager::TryProcessNext()
{
    std::unique_lock<std::recursive_mutex> lock(GetInstance().mMutex);
    if (mRequestQueue.size() > 0)
    {
        mCurrentRequest = mRequestQueue.front();
        mRequestQueue.pop_front();
        std::list<Vector2D> path;
        const bool pathfound = mCurrentRequest->ProcessRequest(mPathfinder.get());
    }
    else
    {
        mConditionVar.wait(lock);
    }
}

void PathFinderManager::FinishedProcessingPath(std::list<Vector2D>& path, bool foundPath) const
{
    mCurrentRequest->NotifyCallback(path, foundPath);
}

PathFinderManager::~PathFinderManager()
{

}

void PathFinderManager::Init(int mapWidth, int mapHeight, float hexSize)
{
    std::lock_guard<std::recursive_mutex> lockGuard(GetInstance().mMutex);
    GetInstance().mPathfinder = std::make_shared<PathSystem::Pathfinding>(static_cast<int>(mapWidth), static_cast<int>(mapHeight), hexSize);
}

PathFinderManager& PathFinderManager::GetInstance()
{
    static PathFinderManager mInstance;
    return mInstance;
}

void PathFinderManager::RequestPath(Vector2D start, Vector2D end, std::function<void(std::list<Vector2D>, bool)> callback)
{
    std::shared_ptr<PathRequest> newRequest = std::make_shared<PathRequest>(start, end, callback);
    PathFinderManager& instance = GetInstance();
    std::lock_guard<std::recursive_mutex> lockGuard(GetInstance().mMutex);
    instance.mRequestQueue.push_back(newRequest);

    instance.mConditionVar.notify_one();
}

void PathFinderManager::RequestPathSmoothing(std::list<Vector2D> path, std::function<void(std::list<Vector2D>, bool)> callback)
{
    std::shared_ptr<PathSmoothRequest> newRequest = std::make_shared<PathSmoothRequest>(path, callback);
    PathFinderManager& instance = GetInstance();
    std::lock_guard<std::recursive_mutex> lockGuard(GetInstance().mMutex);
    instance.mRequestQueue.push_back(newRequest);

    instance.mConditionVar.notify_one();
}

void PathFinderManager::AddObstacle(Vector2D position, Vector2D dimension, float rotation, bool isVisible /*= true*/, bool isHollow /*= false*/)
{
    std::lock_guard<std::recursive_mutex> lockGuard(GetInstance().mMutex);

    if (!isHollow)
    {
        GetInstance().mPathfinder->AddObstacle(position, dimension, rotation * Helper::MathHelper::Deg2Rad);
    }
    else
    {
        std::vector<Vector2D> corners;
        Helper::MathHelper::GetRectangleCorners(position, dimension, rotation * Helper::MathHelper::Deg2Rad, corners);

        GetInstance().mPathfinder->AddBlockedLine(corners[0], corners[1]);
        GetInstance().mPathfinder->AddBlockedLine(corners[1], corners[2]);
        GetInstance().mPathfinder->AddBlockedLine(corners[2], corners[3]);
        GetInstance().mPathfinder->AddBlockedLine(corners[3], corners[0]);
    }
}

void PathFinderManager::Start()
{
    GetInstance().mIsProcessRunning = true;
    while (GetInstance().mIsProcessRunning)
    {
        GetInstance().TryProcessNext();
    }
}

void PathFinderManager::Stop()
{
    std::lock_guard<std::recursive_mutex> lockGuard(GetInstance().mMutex);
    GetInstance().mIsProcessRunning = false;
}

#if !RELEASE
Grid::Layout PathFinderManager::GetLayout()
{
    std::lock_guard<std::recursive_mutex> lockGuard(GetInstance().mMutex);
    return GetInstance().mPathfinder->GetLayout();
}
#endif


PathFinderManager::PathRequest::PathRequest(Vector2D start, Vector2D end, std::function<void(std::list<Vector2D>, bool)> callback) :
    Request(callback), mPathStart(start), mPathEnd(end)
{
}

bool PathFinderManager::PathRequest::ProcessRequest(const PathSystem::Pathfinding* pathfinder)
{
    std::list<Vector2D> path;
    bool pathfound = pathfinder->GetPath(mPathStart, mPathEnd, path);
    PathFinderManager::GetInstance().FinishedProcessingPath(path, pathfound);
    return pathfound;
}

Request::Request(std::function<void(std::list<Vector2D>, bool)> callback) : mCallback(callback)
{

}

void Request::NotifyCallback(std::list<Vector2D> path, bool pathfound)
{
    mCallback(path, pathfound);
}

PathFinderManager::PathSmoothRequest::PathSmoothRequest(std::list<Vector2D> path, std::function<void(std::list<Vector2D>, bool)> callback):
    Request(callback), mPath(path)
{

}

bool PathFinderManager::PathSmoothRequest::ProcessRequest(const PathSystem::Pathfinding* pathfinder)
{
    const bool requestProcessed = true;
    pathfinder->SmoothPath(mPath);
    PathFinderManager::GetInstance().FinishedProcessingPath(mPath, requestProcessed);
    return requestProcessed;
}

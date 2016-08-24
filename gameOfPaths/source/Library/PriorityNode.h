#pragma once
namespace std
{
    template<typename T>
    struct PriorityNode
    {
        T mPayload;
        float mPriority;

        PriorityNode(const T& payload, float priority) : mPayload(payload), mPriority(priority) {}
        PriorityNode& operator=(const PriorityNode& other)
        {
            mPayload = other.mPayload;
            mPriority = other.mPriority;

            return *this;
        }
    };

    template<typename T>
    struct less<PriorityNode<T>>
    {
    public:
        bool operator()(const PriorityNode<T>& lhs, const PriorityNode<T>& rhs) const
        {
            return lhs.mPriority > rhs.mPriority;
        }
    };
}

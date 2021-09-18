#ifndef PRIORITYQUEUENODE_H
#define PRIORITYQUEUENODE_H

template<class T>
struct PriorityQueueNode
{
    T data;
    int priority;
};

template<typename T>
bool operator<(PriorityQueueNode<T> left, PriorityQueueNode<T> right)
{
    return left.priority < right.priority;
}

template<typename T>
bool operator>(PriorityQueueNode<T> left, PriorityQueueNode<T> right)
{
    return left.priority > right.priority;
}

template<typename T>
bool operator<=(PriorityQueueNode<T> left, PriorityQueueNode<T> right)
{
    return left.priority <= right.priority;
}

template<typename T>
bool operator>=(PriorityQueueNode<T> left, PriorityQueueNode<T> right)
{
    return left.priority >= right.priority;
}
#endif // PRIORITYQUEUENODE_H

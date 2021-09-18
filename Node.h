#ifndef NODE_H
#define NODE_H

template<class T>
struct Node
{
    T data;
    Node* left;
    Node* right;
};
#endif // NODE_H

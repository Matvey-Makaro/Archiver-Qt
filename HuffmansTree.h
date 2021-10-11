#ifndef HUFFMANSTREE_H
#define HUFFMANSTREE_H

#include<string>
#include<vector>
#include<queue>
#include"Node.h"
#include"PriorityQueueNode.h"

class HuffmansTree
{
public:
    HuffmansTree();
    void create();
    int createHuffTable(std::vector<std::string>& huffTable);
    void push(unsigned char value, int priority);
    Node<unsigned char>* getRoot();
    int size() const;
    bool isLeaf(Node<unsigned char>* node) const;
    void clear(); //очистить выделенную память
    ~HuffmansTree(); //очистить выделенную память
private:
    Node<unsigned char>* root;
    std::priority_queue<PriorityQueueNode<Node<unsigned char>*>, std::vector<PriorityQueueNode<Node<unsigned char>*>>, std::greater<PriorityQueueNode<Node<unsigned char>*>>> queue;
    int numNodes;
private:
    Node<unsigned char>* deleteTree(Node<unsigned char>* tmpNode);
    Node<unsigned char>* fillHuffTable(Node<unsigned char>* tmpNode, std::string code, std::vector<std::string>& huffTable);
};
#endif // HUFFMANSTREE_H

#include<iostream> //удалить
#include"HuffmansTree.h"

HuffmansTree::HuffmansTree() :
    root(nullptr), numNodes(0) {}

void HuffmansTree::create()
{
    if (queue.empty())
        return;
    Node<unsigned char>* tmp;
    PriorityQueueNode<Node<unsigned char>*> firstBuffer, secondBuffer, thirdBuffer;
    while (queue.size() != 1)
    {
        tmp = new Node<unsigned char>;
        tmp->data = 0;
        firstBuffer = queue.top();
        queue.pop();
        secondBuffer = queue.top();
        queue.pop();
        tmp->left = firstBuffer.data;
        tmp->right = secondBuffer.data;
        thirdBuffer.data = tmp;
        thirdBuffer.priority = firstBuffer.priority + secondBuffer.priority;
        queue.push(thirdBuffer);
    }
    root = queue.top().data;
    queue.pop();
}

int HuffmansTree::createHuffTable(std::vector<std::string>& huffTable)
{
    if (root)
    {
        std::string code;
        if (isLeaf(root))
        {
            code.push_back('0');
            huffTable[root->data] = code;
            return 0;
        }
        else
        {
            fillHuffTable(root, code, huffTable);
//            for (short i = 0; i < huffTable.size(); ++i)	//для отладки, выводит новые коды для символов
//            {
//                if (!huffTable[i].empty())
//                    std::cout << (unsigned char)i << " " << huffTable[i] << std::endl;
//            }
            return 1;
        }
    }
    return 0;
}

void HuffmansTree::push(unsigned char value, int priority)
{
    PriorityQueueNode<Node<unsigned char>*> buffer;
    Node<unsigned char>* tmp = new Node<unsigned char>;
    tmp->data = value;
    tmp->left = nullptr;
    tmp->right = nullptr;
    buffer.data = tmp;
    buffer.priority = priority;
    queue.push(buffer);
    ++numNodes;
}

Node<unsigned char>* HuffmansTree::getRoot() { return root; }

int HuffmansTree::size() const { return numNodes; }

bool HuffmansTree::isLeaf(Node<unsigned char>* node) const
{
    if (node == nullptr)
        return false;
    else
        return node->left == nullptr && node->right == nullptr;
}

void HuffmansTree::clear()
{
    Node<unsigned char>* tmpRoot = root;
    if (root != nullptr)
    {
        deleteTree(tmpRoot);
        root = nullptr;
    }
    numNodes = 0;
}

HuffmansTree::~HuffmansTree() { clear(); }

Node<unsigned char>* HuffmansTree::deleteTree(Node<unsigned char>* tmpNode)
{
    if (tmpNode == nullptr) return nullptr;
    deleteTree(tmpNode->left);
    deleteTree(tmpNode->right);
    delete tmpNode;
    return nullptr;
}

Node<unsigned char>* HuffmansTree::fillHuffTable(Node<unsigned char>* tmpNode, std::string code, std::vector<std::string>& huffTable)
{
    if (isLeaf(tmpNode))
    {
        huffTable[tmpNode->data] = code;
        return tmpNode;
    }
    code.push_back('0');
    fillHuffTable(tmpNode->left, code, huffTable);
    code.pop_back();
    code.push_back('1');
    fillHuffTable(tmpNode->right, code, huffTable);
    return nullptr;
}

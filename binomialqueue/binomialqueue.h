#ifndef BINOMIALQUEUE_H
#define BINOMIALQUEUE_H

#define MaxSize 8
#define Capacity 511

class BinomialQueue
{
public:
    BinomialQueue();
    BinomialQueue(int val);

    void merge(BinomialQueue &rhs);
    void insert(int val);
    void deleteMin();
    int findMin();
    void printQueue();
private:
    struct BinomialNode{
        int key;
        BinomialNode *firstChild;
        BinomialNode *nextSibling;

        BinomialNode(int val)
            :key(val), firstChild(0), nextSibling(0){}
    };

    int currentSize;
    BinomialNode *theTrees[MaxSize];

    BinomialNode* combineTree(BinomialNode *lhs, BinomialNode *rhs);
    void printQueue(BinomialNode *q);
};

#endif // BINOMIALQUEUE_H

#ifndef FIBHEAP_H
#define FIBHEAP_H

#define Min -1


class FibHeap{
private:
    struct FibNode{
        int key;
        int degree;
        FibNode *child;
        FibNode *parent;
        FibNode *left;
        FibNode *right;
        bool marked;

        FibNode(int val)
            :key(val), degree(0), child(0), parent(0),
              left(0), right(0), marked(false){}
    };

    FibNode *min;
    int size;
    FibNode **tempArray;

    void consolidate();
    FibNode *fibHeapLink(FibNode *lhs, FibNode *rhs);
    void fibHeapInsert(FibNode *h);
    void cut(FibNode *node, FibNode *parent);
    void cascadingCut(FibNode *node);
    void fibHeapDecreaseKey(FibNode *node, const int val);
    void fibHeapDelete(FibNode *node);

    void print(FibNode *node, FibNode *prev, int direction);
    FibNode* search(FibNode *h, const int key);

    void deleteDoublyLinkedList(FibNode *node);
    void insertDoublyLinkedList(FibNode *&l, FibNode *node);

public:
    FibHeap();
    FibNode* fibHeapExtractMin();
    void fibHeapInsert(const int val);
    void fibHeapUnion(FibHeap *h);
    void fibHeapDecreaseKey(const int val, const int newVal);
    void fibHeapDelete(const int val);

    void print();
    FibNode* search(const int key);
};

#endif // FIBHEAP_H

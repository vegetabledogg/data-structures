#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <iostream>
using namespace std;

class RedBlackTree
{
public:
    RedBlackTree();

    void insert(int val);
    void remove(int val);
    void printTree() const;

    enum {RED, BLACK};
private:
    struct RedBlackNode{
        int key;
        RedBlackNode *left;
        RedBlackNode *right;
        RedBlackNode *parent;
        int color;

        RedBlackNode(int val, RedBlackNode *lt, RedBlackNode *rt, RedBlackNode *pt, int nodeColor)
            :key(val), left(lt), right(rt), parent(pt), color(nodeColor){}
    };

    RedBlackNode *root;
    RedBlackNode *nullNode;

    RedBlackNode* findMax(RedBlackNode *t);
    RedBlackNode* findMin(RedBlackNode *t);
    void leftRotate(RedBlackNode *t);
    void rightRotate(RedBlackNode *t);
    void transplant(RedBlackNode *t, RedBlackNode *fixNode);
    void insertFixUp(RedBlackNode *t);
    void removeFixUp(RedBlackNode *t);
    void preTraversal(RedBlackNode *t) const;
    void inTraversal(RedBlackNode *t) const;
};

#endif // REDBLACKTREE_H

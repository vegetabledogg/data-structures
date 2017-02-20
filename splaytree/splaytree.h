#ifndef SPLAYTREE_H
#define SPLAYTREE_H


#include <iostream>
using namespace std;

class SplayTree
{
public:
    SplayTree();
    ~SplayTree();
    void insert(int val);
    void remove(int val);
    int findMax() const;
    int findMin() const;
    void printTree() const;
private:
    struct SplayNode{
        int key;
        SplayNode *left;
        SplayNode *right;
        SplayNode *parent;

        SplayNode(int val, SplayNode *lt, SplayNode *rt, SplayNode *p)
            :key(val), left(lt), right(rt), parent(p){}
    };

    SplayNode *root;

    void zig(SplayNode *t);
    void zag(SplayNode *t);
    void splay(SplayNode *t);
    SplayNode* findMax(SplayNode *t) const;
    SplayNode* findMin(SplayNode *t) const;
    SplayNode* find(int val, SplayNode *t) const;
    void join(SplayNode *lhs, SplayNode *rhs);
    void printTree(SplayNode *t) const;
    void makeEmpty(SplayNode *&t);
};

#endif // SPLAYTREE_H

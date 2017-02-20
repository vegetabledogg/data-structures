#ifndef AVLTREE_H
#define AVLTREE_H

int max(const int a, const int b);

class AVLTree
{
public:
    AVLTree();
    ~AVLTree();
    int findMax() const;
    int findMin() const;
    inline int getHeight();
    void insertNode(const int val);
    void deleteNode(const int val);
    void printTree();

    const AVLTree& operator=(const AVLTree &rhs);
private:
    struct AVLNode{
        int key;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode(const int val, AVLNode *lt, AVLNode *rt, int h = 0)
            :key(val), left(lt), right(rt), height(h){}
    };

    AVLNode *root;

    void leftLeftRotate(AVLNode *&t);
    void leftRightRotate(AVLNode *&t);
    void rightLeftRotate(AVLNode *&t);
    void rightRightRotate(AVLNode *&t);

    AVLNode* findMin(AVLNode *t) const;
    AVLNode* findMax(AVLNode *t) const;
    int getHeight(AVLNode *t) const;

    void insertNode(int val, AVLNode *&t);
    void deleteNode(int val, AVLNode *&t);

    void printTree(AVLNode *t) const;
    void makeEmpty(AVLNode *&t);
    AVLNode* clone(AVLNode *t);
};

inline int AVLTree::getHeight(){
    return getHeight(root);
}

#endif // AVLTREE_H

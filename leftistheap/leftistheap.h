#ifndef LEFTISTHEAP_H
#define LEFTISTHEAP_H

int min(int a, int b);

class LeftistHeap
{
public:
    LeftistHeap();
    void merge(LeftistHeap &rhs);
    void insert(int val);
    void deleteMin();
    void printHeap() const;
private:
    struct LeftistNode{
        int key;
        LeftistNode *left;
        LeftistNode *right;
        int npl;

        LeftistNode(int val, LeftistNode *lt, LeftistNode *rt, int n)
            :key(val), left(lt), right(rt), npl(n){}
    };

    LeftistNode *root;

    void swapChild(LeftistNode *t);
    int getNpl(const LeftistNode *t) const;
    LeftistNode* merge(LeftistNode *lhs, LeftistNode *rhs);

    void preorder(const LeftistNode *t) const;
    void inorder(const LeftistNode *t) const;
};

#endif // LEFTISTHEAP_H

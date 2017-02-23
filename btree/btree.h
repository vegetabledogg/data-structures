#ifndef BTREE_H
#define BTREE_H

#define MinDegree 3

class BTree
{
public:
    BTree();

    void insert(int val);
    void deleteKey(int val);
    void printTree();
private:
    struct BNode{
        int keyNum;
        int key[MinDegree * 2 - 1];
        BNode* child[MinDegree * 2];
        bool isLeaf;

        BNode(int size, bool isLeafNode)
            :keyNum(size), isLeaf(isLeafNode){
            for(int i = 0; i < MinDegree * 2; i++){
                child[i] = 0;
            }
        }
    };

    BNode *root;

    void shift(int key[], bool direction, int begin, int end);
    void shift(BNode* child[], bool direction, int begin, int end);
    int getIndex(BNode *t, int val);
    void split(BNode *t, int index);
    void insertFix(BNode *t, int val);
    void merge(BNode *t, int lhs);
    void deleteFix(BNode *t, int val);
    void printTree(BNode *t);
};

#endif // BTREE_H

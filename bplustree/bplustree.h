#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define MinDegree 3

class BplusTree
{
public:
    BplusTree();

    void insert(int val);
    void deleteKey(int val);

    void printTree();
private:
    struct BplusNode{
        int keyNum;
        int key[MinDegree * 2 - 1];
        BplusNode* child[MinDegree * 2];
        bool isLeaf;
        BplusNode *next;

        BplusNode(int size, int isLeafNode)
            :keyNum(size), isLeaf(isLeafNode), next(0){
            int i;
            for(i = 0; i < MinDegree * 2; i++){
                child[i] = 0;
            }
        }
    };

    BplusNode *root;

    void shift(int key[], bool direction, int begin, int end);
    void shift(BplusNode* child[], bool direction, int begin, int end);

    void spilit(BplusNode *t, int spilitIndex);
    void merge(BplusNode *t, int lhs, int rhs);
    int search(BplusNode *t, int val);

    void insertFix(BplusNode *t, int val);
    void deleteFix(BplusNode *t, int val);

    void printTree(BplusNode *t);
};

#endif // BPLUSTREE_H

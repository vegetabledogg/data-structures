#include "bplustree.h"
#include <iostream>
using namespace std;

BplusTree::BplusTree()
    :root(nullptr){
    root = new BplusNode(0, true);
}

void BplusTree::shift(int key[], bool direction, int begin, int end){
    int i;

    if(false == direction){
        for(i = begin; i <= end; i++){
            key[i - 1] = key[i];
        }
    }
    else{
        for(i = end; i >= begin; i--){
            key[i + 1] = key[i];
        }
    }
}

void BplusTree::shift(BplusNode *child[], bool direction, int begin, int end){
    int i;

    if(false == direction){
        for(i = begin; i <= end; i++){
            child[i - 1] = child[i];
        }
    }
    else{
        for(i = end; i >= begin; i--){
            child[i + 1] = child[i];
        }
    }
}

void BplusTree::split(BplusNode *t, int splitIndex){
    int i;
    BplusNode *splitNode = t->child[splitIndex];

    if(true == splitNode->isLeaf){
        BplusNode *newNode = new BplusNode(MinDegree - 1, true);
        for(i = 0; i < MinDegree - 1; i++){
            newNode->key[i] = splitNode->key[i + MinDegree];
        }
        newNode->next = splitNode->next;
        splitNode->next = newNode;

        shift(t->key, true, splitIndex, t->keyNum - 1);
        shift(t->child, true, splitIndex + 1, t->keyNum);
        t->key[splitIndex] = splitNode->key[MinDegree - 1];
        t->child[splitIndex + 1] = newNode;
        t->keyNum += 1;

        splitNode->keyNum = MinDegree;
    }
    else{
        BplusNode *newNode = new BplusNode(MinDegree - 1, false);
        for(i = 0; i < MinDegree - 1; i++){
            newNode->key[i] = splitNode->key[i + MinDegree];
        }
        for(i = 0; i < MinDegree; i++){
            newNode->child[i] = splitNode->child[i + MinDegree];
        }

        shift(t->key, true, splitIndex, t->keyNum - 1);
        shift(t->child, true, splitIndex + 1, t->keyNum);
        t->key[splitIndex] = splitNode->key[MinDegree - 1];
        t->child[splitIndex + 1] = newNode;
        t->keyNum += 1;

        splitNode->keyNum = MinDegree - 1;
    }
}

void BplusTree::merge(BplusNode *t, int lhs, int rhs){
    int i;
    BplusNode *left = t->child[lhs];
    BplusNode *right = t->child[rhs];

    if(true == left->isLeaf){
        for(i = 0; i < MinDegree - 1; i++){
            left->key[i + MinDegree - 1] = right->key[i];
        }
        left->keyNum = MinDegree * 2 - 2;
        left->next = right->next;

        shift(t->key, false, rhs, t->keyNum - 1);
        shift(t->child, false, rhs + 1, t->keyNum);
        t->keyNum -= 1;

        delete right;
    }
    else{
        for(i = 0; i < MinDegree - 1; i++){
            left->key[i + MinDegree] = right->key[i];
        }
        for(i = 0; i < MinDegree; i++){
            left->child[i + MinDegree] = right->child[i];
        }
        left->key[MinDegree - 1] = t->key[lhs];
        left->keyNum = MinDegree * 2 - 1;

        shift(t->key, false, rhs, t->keyNum - 1);
        shift(t->child, false, rhs + 1, t->keyNum);
        t->keyNum -= 1;

        delete right;
    }

    if(0 == t->keyNum && t == root){
        BplusNode *oldNode = root;
        root = root->child[0];
        delete oldNode;
    }
}

int BplusTree::search(BplusNode *t, int val){
    int i;

    for(i = 0; i < t->keyNum; i++){
        if(t->key[i] >= val){
            break;
        }
    }
    return i;
}

void BplusTree::insertFix(BplusNode *t, int val){
    int index = search(t, val);

    if(true == t->isLeaf){
        shift(t->key, true, index, t->keyNum - 1);
        t->key[index] = val;
        t->keyNum += 1;
    }
    else{
        if(t->child[index]->keyNum == MinDegree * 2 - 1){
            split(t, index);
            if(val > t->key[index]){
                index++;
            }
        }
        insertFix(t->child[index], val);
    }
}

void BplusTree::insert(int val){
    if(root->keyNum == MinDegree * 2 - 1){
        BplusNode *newNode = new BplusNode(0, false);
        newNode->child[0] = root;
        root = newNode;
    }
    insertFix(root, val);
}

void BplusTree::deleteFix(BplusNode *t, int val){
    int index = search(t, val);

    if(true == t->isLeaf){
        if(index < t->keyNum && t->key[index] == val){
            shift(t->key, false, index + 1, t->keyNum - 1);
            t->keyNum -= 1;
        }
    }
    else{
        BplusNode *leftSibling = nullptr, *subNode, *rightSibling = nullptr;
        if(index > 0){
            leftSibling = t->child[index - 1];
        }
        subNode = t->child[index];
        if(index < t->keyNum){
            rightSibling = t->child[index + 1];
        }

        if(index < t->keyNum && t->key[index] == val){
            if(true == subNode->isLeaf){
                if(subNode->keyNum >= MinDegree){
                    t->key[index] = subNode->key[subNode->keyNum - 2];
                    subNode->keyNum -= 1;
                }
                else if(rightSibling->keyNum >= MinDegree){
                    t->key[index] = rightSibling->key[0];
                    subNode->key[subNode->keyNum - 1] = t->key[index];
                    deleteFix(rightSibling, t->key[index]);
                }
                else{
                    merge(t, index, index + 1);
                    deleteFix(subNode, val);
                }
            }
            else{
                if(subNode->keyNum >= MinDegree){
                    t->key[index] = subNode->key[subNode->keyNum - 1];
                    deleteFix(subNode, t->key[index]);
                }
                else if(rightSibling->keyNum >= MinDegree){
                    t->key[index] = rightSibling->key[0];
                    deleteFix(rightSibling, t->key[index]);
                }
                else{
                    merge(t, index, index + 1);
                    deleteFix(subNode, val);
                }
            }
        }
        else{
            if(true == subNode->isLeaf){
                if(subNode->keyNum >= MinDegree){
                    deleteFix(subNode, val);
                }
                else if(nullptr != leftSibling && leftSibling->keyNum >= MinDegree){
                    shift(subNode->key, true, 0, subNode->keyNum - 1);
                    subNode->key[0] = t->key[index - 1];
                    subNode->keyNum += 1;

                    t->key[index - 1] = leftSibling->key[leftSibling->keyNum - 2];
                    leftSibling->keyNum -= 1;

                    deleteFix(subNode, val);
                }
                else if(nullptr != rightSibling && rightSibling->keyNum >= MinDegree){
                    t->key[index] = rightSibling->key[0];
                    subNode->key[subNode->keyNum] = t->key[index];
                    subNode->keyNum += 1;

                    shift(rightSibling->key, false, 1, rightSibling->keyNum - 1);
                    rightSibling->keyNum -= 1;

                    deleteFix(subNode, val);
                }
                else{
                    if(nullptr != leftSibling){
                        merge(t, index - 1, index);
                        deleteFix(leftSibling, val);
                    }
                    else{
                        merge(t, index, index + 1);
                        deleteFix(subNode, val);
                    }
                }
            }
            else{
                if(subNode->keyNum >= MinDegree){
                    deleteFix(subNode, val);
                }
                else if(nullptr != leftSibling && leftSibling->keyNum >= MinDegree){
                    shift(subNode->key, true, 0, subNode->keyNum - 1);
                    shift(subNode->child, true, 0, subNode->keyNum);
                    subNode->key[0] = t->key[index - 1];
                    t->key[index - 1] = leftSibling->key[leftSibling->keyNum - 1];
                    subNode->child[0] = leftSibling->child[leftSibling->keyNum];
                    subNode->keyNum += 1;
                    leftSibling->keyNum -= 1;
                    deleteFix(subNode, val);
                }
                else if(nullptr != rightSibling && rightSibling->keyNum >= MinDegree){
                    subNode->key[subNode->keyNum] = t->key[index];
                    t->key[index] = rightSibling->key[0];
                    subNode->child[subNode->keyNum + 1] = rightSibling->child[0];
                    subNode->keyNum += 1;
                    shift(rightSibling->key, false, 1, rightSibling->keyNum - 1);
                    shift(rightSibling->child, false, 1, rightSibling->keyNum);
                    rightSibling->keyNum -= 1;
                    deleteFix(subNode, val);
                }
                else{
                    if(nullptr != leftSibling){
                        merge(t, index - 1, index);
                        deleteFix(leftSibling, val);
                    }
                    else{
                        merge(t, index, index + 1);
                        deleteFix(subNode, val);
                    }
                }
            }
        }
    }
}

void BplusTree::deleteKey(int val){
    if(0 == root->keyNum){
        return;
    }
    deleteFix(root, val);
}

void BplusTree::printTree(){
    printTree(root);
}

void BplusTree::printTree(BplusNode *t){
    int i;

    if(nullptr == t){
        return;
    }

    cout << "[";
    for(i = 0; i < t->keyNum; i++){
        cout << t->key[i];
        if(i != t->keyNum - 1){
            cout << " ";
        }
    }
    cout << "]";
    cout << t->isLeaf << " ";
    cout << t->keyNum << " ";
    cout << endl;

    for(i = 0; i <= t->keyNum; i++){
        printTree(t->child[i]);
    }
}

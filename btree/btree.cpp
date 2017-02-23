#include "btree.h"
#include <iostream>
using namespace std;

BTree::BTree()
    :root(nullptr){
    root = new BNode(0, true);
}

void BTree::shift(int key[], bool direction, int begin, int end){
    int i;
    if(true == direction){
        for(i = end; i >= begin; i--){
            key[i + 1] = key[i];
        }
    }
    else{
        for(i = begin; i <= end; i++){
            key[i - 1] = key[i];
        }
    }
}

void BTree::shift(BNode *child[], bool direction, int begin, int end){
    int i;
    if(true == direction){
        for(i = end; i >= begin; i--){
            child[i + 1] = child[i];
        }
    }
    else{
        for(i = begin; i <= end; i++){
            child[i - 1] = child[i];
        }
    }
}

int BTree::getIndex(BNode *t, int val){
    int i;
    for(i = 0; i < t->keyNum; i++){
        if(t->key[i] >= val){
            break;
        }
    }
    return i;
}

void BTree::spilit(BNode *t, int index){
    int i;
    BNode *subNode = t->child[index];
    BNode *newNode = new BNode(MinDegree - 1, subNode->isLeaf);
    for(i = 0; i < MinDegree - 1; i++){
        newNode->key[i] = subNode->key[i + MinDegree];
    }
    if(false == subNode->isLeaf){
        for(i = 0; i < MinDegree; i++){
            newNode->child[i] = subNode->child[i + MinDegree];
        }
    }
    shift(t->key, true, index, t->keyNum - 1);
    shift(t->child, true, index + 1, t->keyNum);
    t->key[index] = subNode->key[MinDegree - 1];
    t->child[index + 1] = newNode;
    t->keyNum += 1;
    subNode->keyNum = MinDegree - 1;
}

void BTree::insertFix(BNode *t, int val){
    int index = getIndex(t, val);
    if(true == t->isLeaf){
        shift(t->key, true, index, t->keyNum - 1);
        t->key[index] = val;
        t->keyNum += 1;
    }
    else{
        if(t->child[index]->keyNum == MinDegree * 2 - 1){
            spilit(t, index);
            if(val > t->key[index]){
                index += 1;
            }
        }
        insertFix(t->child[index], val);
    }
}

void BTree::insert(int val){
    if(root->keyNum == MinDegree * 2 - 1){
        BNode *newNode = new BNode(0, false);
        newNode->child[0] = root;
        root = newNode;
    }
    insertFix(root, val);
}

void BTree::merge(BNode *t, int lhs){
    int i;
    BNode *leftNode = t->child[lhs], *rightNode = t->child[lhs + 1];
    for(i = 0; i < rightNode->keyNum; i++){
        leftNode->key[i + leftNode->keyNum + 1] = rightNode->key[i];
    }
    if(false == leftNode->isLeaf){
        for(i = 0; i < rightNode->keyNum + 1; i++){
            leftNode->child[i + leftNode->keyNum + 1] = rightNode->child[i];
        }
    }
    leftNode->key[leftNode->keyNum] = t->key[lhs];
    leftNode->keyNum += rightNode->keyNum + 1;
    shift(t->key, false, lhs + 1, t->keyNum - 1);
    shift(t->child, false, lhs + 2, t->keyNum);
    t->keyNum -= 1;
    delete rightNode;
    if(0 == t->keyNum && t == root){
        root = leftNode;
    }
}

void BTree::deleteFix(BNode *t, int val){
    int index = getIndex(t, val);

    if(true == t->isLeaf){
        if(index < t->keyNum && t->key[index] == val){
            shift(t->key, false, index + 1, t->keyNum - 1);
            t->keyNum -= 1;
        }
    }
    else{
        BNode *leftNode, *subNode, *rightNode;
        if(index > 0){
            leftNode = t->child[index - 1];
        }
        subNode = t->child[index];
        if(index < t->keyNum){
            rightNode = t->child[index + 1];
        }

        if(index < t->keyNum && t->key[index] == val){
            if(subNode->keyNum >= MinDegree){
                t->key[index] = subNode->key[subNode->keyNum - 1];
                deleteFix(subNode, t->key[index]);
            }
            else if(rightNode->keyNum >= MinDegree){
                t->key[index] = rightNode->key[0];
                deleteFix(rightNode, t->key[index]);
            }
            else{
                merge(t, index);
                deleteFix(subNode, val);
            }
        }
        else{
            if(subNode->keyNum >= MinDegree){
                deleteFix(subNode, val);
            }
            else if(index > 0 && leftNode->keyNum >= MinDegree){
                shift(subNode->key, true, 0, subNode->keyNum - 1);
                subNode->key[0] = t->key[index - 1];
                if(false == subNode->isLeaf){
                    shift(subNode->child, true, 0, subNode->keyNum);
                    subNode->child[0] = leftNode->child[leftNode->keyNum];
                }
                subNode->keyNum += 1;
                t->key[index - 1] = leftNode->key[leftNode->keyNum - 1];
                leftNode->keyNum -= 1;
                deleteFix(subNode, val);
            }
            else if(index < t->keyNum && rightNode->keyNum >= MinDegree){
                subNode->key[subNode->keyNum] = t->key[index];
                if(false == subNode->isLeaf){
                    subNode->child[subNode->keyNum + 1] = rightNode->child[0];
                }
                subNode->keyNum += 1;
                t->key[index] = rightNode->key[0];
                shift(rightNode->key, false, 1, rightNode->keyNum - 1);
                if(false == subNode->isLeaf){
                    shift(rightNode->child, false, 1, rightNode->keyNum);
                }
                rightNode->keyNum -= 1;
                deleteFix(subNode, val);
            }
            else{
                if(index > 0){
                    merge(t, index - 1);
                    deleteFix(leftNode, val);
                }
                else{
                    merge(t, index);
                    deleteFix(subNode, val);
                }
            }
        }
    }
}

void BTree::deleteKey(int val){
    if(root->keyNum > 0){
        deleteFix(root, val);
    }
}

void BTree::printTree(){
    printTree(root);
}

void BTree::printTree(BNode *t){
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

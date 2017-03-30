#include "fibheap.h"
#include <iostream>
#include <cmath>
using namespace std;

FibHeap::FibHeap()
    :min(0), size(0), tempArray(0)
{

}

void FibHeap::fibHeapInsert(const int val){
    FibNode *node = new FibNode(val);
    fibHeapInsert(node);
    size++;
}

void FibHeap::fibHeapInsert(FibNode *h){
    insertDoublyLinkedList(min, h);
    if(min->key > h->key){
        min = h;
    }
    h->parent = nullptr;
    h->marked = false;
}

void FibHeap::fibHeapUnion(FibHeap *h){
    if(nullptr == h || nullptr == h->min){
        return;
    }
    else if(nullptr == min){
        min = h->min;
    }
    else{
        FibNode *node = h->min;
        FibNode *tempNode = node->right;
        node->right = min->right;
        min->right->left = node;
        min->right = tempNode;
        tempNode->left = min;
        if(min->key > node->key){
            min = node;
        }
    }
    size += h->size;
    delete h;
}

FibHeap::FibNode *FibHeap::fibHeapExtractMin(){
    FibNode *minNode = min;
    if(nullptr != min){
        FibNode *pNode = min->child;
        if(nullptr != pNode){
            FibNode *tempNode;
            do{
                tempNode = pNode;
                pNode = pNode->right;
                fibHeapInsert(tempNode);
            }while(pNode != min->child);
        }
        if(minNode == minNode->right){
            min = nullptr;
        }
        else{
            min = minNode->right;
            deleteDoublyLinkedList(minNode);
            consolidate();
        }
        size--;
    }
    return minNode;
}

void FibHeap::consolidate(){
    int maxDegree = (log(size)/log(2.0)) + 1;
    tempArray = (FibNode**)malloc((maxDegree + 1) * sizeof(FibNode*));
    for(int i = 0; i <= maxDegree; i++){
        tempArray[i] = nullptr;
    }
    FibNode *tempNode;
    while(nullptr != min){
        tempNode = min;
        if(min == min->right){
            min = nullptr;
        }
        else{
            min = min->right;
            deleteDoublyLinkedList(tempNode);
        }
        tempNode->left = tempNode->right = tempNode;
        int degree = tempNode->degree;
        while(nullptr != tempArray[degree]){
            if(tempNode->key < tempArray[degree]->key){
                tempNode = fibHeapLink(tempNode, tempArray[degree]);
            }
            else{
                tempNode = fibHeapLink(tempArray[degree], tempNode);
            }
            tempArray[degree] = nullptr;
            degree++;
        }
        tempArray[degree] = tempNode;
        //for test
        //cout << "degree[" << degree << "]: " << tempArray[degree]->key << endl;
    }
    for(int i = 0; i <= maxDegree; i++){
        if(nullptr != tempArray[i]){
            fibHeapInsert(tempArray[i]);
        }
    }
    delete tempArray;
    tempArray = nullptr;
}

FibHeap::FibNode *FibHeap::fibHeapLink(FibNode *lhs, FibNode *rhs){
    insertDoublyLinkedList(lhs->child, rhs);
    lhs->degree += 1;
    rhs->parent = lhs;
    rhs->marked = false;
    return lhs;
}

void FibHeap::cut(FibNode *node, FibNode *parent){
    if(node->right == node){
        parent->child = nullptr;
    }
    else{
        if(parent->child == node){
            parent->child = node->right;
        }
        deleteDoublyLinkedList(node);
    }
    parent->degree -= 1;
    fibHeapInsert(node);
}

void FibHeap::cascadingCut(FibNode *node){
    FibNode *parent = node->parent;
    if(nullptr != parent){
        if(false == node->marked){
            node->marked = true;
        }
        else{
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

void FibHeap::fibHeapDecreaseKey(FibNode *node, const int val){
    if(node->key < val){
        return;
    }
    FibNode *parent = node->parent;
    node->key = val;
    if(nullptr != parent && node->key < parent->key){
        cut(node, parent);
        cascadingCut(parent);
    }
    if(node->key < min->key){
        min = node;
    }
}

void FibHeap::fibHeapDecreaseKey(const int val, const int newVal){
    FibNode *node = search(val);
    if(nullptr == node){
        return;
    }
    fibHeapDecreaseKey(node, newVal);
}

void FibHeap::fibHeapDelete(FibNode *node){
    fibHeapDecreaseKey(node, Min);
    fibHeapExtractMin();
}

void FibHeap::fibHeapDelete(const int val){
    FibNode *node = search(val);
    if(nullptr == node){
        return;
    }
    fibHeapDelete(node);
}

void FibHeap::print(FibNode *node, FibNode *prev, int direction)
{
    FibNode *start = node;

    if(nullptr == node)
        return ;
    do{
        if(direction == 1){
            cout << node->key << "(" << node->degree << ") is "<< prev->key << "'s child" << endl;
        }
        else{
            cout << node->key << "(" << node->degree << ") is "<< prev->key << "'s next" << endl;
        }
        if(nullptr != node->child){
            print(node->child, node, 1);
        }
        prev = node;
        node = node->right;
        direction = 2;
    }while(node != start);
}

void FibHeap::print()
{
    int i = 0;
    FibNode *pNode = min;

    if(nullptr == min){
        return;
    }
    cout << "== 斐波那契堆的详细信息: ==" << endl;
    do{
        i++;
        cout << i << ". " << pNode->key << "(" << pNode->degree << ") is root" << endl;

        print(pNode->child, pNode, 1);
        pNode = pNode->right;
    }while(pNode != min);
    cout << endl;
}

FibHeap::FibNode* FibHeap::search(FibNode *h, const int key)
{
    FibNode *pNode = h;
    FibNode *result = nullptr;

    if(nullptr == h){
        return nullptr;
    }
    do{
        if(pNode->key == key){
            result = pNode;
            break;
        }
        else{
            if(nullptr != (result = search(pNode->child, key))){
                break;
            }
        }
        pNode = pNode->right;
    }while(pNode != h);

    //for test
    /*
    if(nullptr != result){
        cout << ">>> Test for function search: " << result->key << endl;
    }
    else{
        cout << ">>> Test for function search: nullptr" << endl;
    }
    */
    return result;
}

FibHeap::FibNode *FibHeap::search(const int key)
{
    if(nullptr == min){
        return nullptr;
    }
    return search(min, key);
}

void FibHeap::deleteDoublyLinkedList(FibNode *node){
    node->left->right = node->right;
    node->right->left = node->left;
}

void FibHeap::insertDoublyLinkedList(FibNode *&l, FibNode *node){
    if(nullptr == l){
        l = node;
        node->left = node->right = node;
    }
    else{
        node->right = l->right;
        node->left = l;
        l->right->left = node;
        l->right = node;
    }
}

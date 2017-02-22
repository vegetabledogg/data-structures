#include "binomialqueue.h"
#include <iostream>
#include <tgmath.h>
using namespace std;

BinomialQueue::BinomialQueue()
    :currentSize(0){
    for(int i = 0; i < MaxSize; i++){
        theTrees[i] = nullptr;
    }
}

BinomialQueue::BinomialQueue(int val)
    :currentSize(1){
    for(int i = 0; i < MaxSize; i++){
        if(0 == i){
            theTrees[i] = new BinomialNode(val);
        }
        else{
            theTrees[i] = nullptr;
        }
    }
}

BinomialQueue::BinomialNode* BinomialQueue::combineTree(BinomialNode *lhs, BinomialNode *rhs){
    if(lhs->key <= rhs->key){
        rhs->nextSibling = lhs->firstChild;
        lhs->firstChild = rhs;
        return lhs;
    }
    else{
        return combineTree(rhs, lhs);
    }
}

void BinomialQueue::merge(BinomialQueue &rhs){
    if(this == &rhs){
        return;
    }
    currentSize += rhs.currentSize;
    if(currentSize > Capacity){
        currentSize -= rhs.currentSize;
        cout << "Can not merge!" << endl;
    }
    BinomialNode *carry = nullptr;
    for(int i = 0; i < MaxSize; i++){
        int caseNum = 0;
        if(nullptr != carry){
            caseNum += 1;
        }
        if(nullptr != theTrees[i]){
            caseNum += 2;
        }
        if(nullptr != rhs.theTrees[i]){
            caseNum += 4;
        }
        switch(caseNum){
        case 0:
            break;
        case 1:
            theTrees[i] = carry;
            carry = nullptr;
            break;
        case 2:
            break;
        case 3:
            carry = combineTree(carry, theTrees[i]);
            theTrees[i] = nullptr;
            break;
        case 4:
            theTrees[i] = rhs.theTrees[i];
            rhs.theTrees[i] = nullptr;
            break;
        case 5:
            carry = combineTree(carry, rhs.theTrees[i]);
            rhs.theTrees[i] = nullptr;
            break;
        case 6:
            carry = combineTree(theTrees[i], rhs.theTrees[i]);
            theTrees[i] = nullptr;
            rhs.theTrees[i] = nullptr;
            break;
        case 7:
            carry = combineTree(carry, rhs.theTrees[i]);
            rhs.theTrees[i] = nullptr;
            break;
        default:
            break;
        }
    }
}

void BinomialQueue::insert(int val){
    BinomialQueue newQueue(val);
    merge(newQueue);
}

int BinomialQueue::findMin(){
    int minIndex = 0, min = 1000000;

    for(int i = 0; i < MaxSize; i++){
       if(nullptr != theTrees[i] && theTrees[i]->key < min){
           min = theTrees[i]->key;
           minIndex = i;
       }
    }
    return minIndex;
}

void BinomialQueue::deleteMin(){
    if(0 == currentSize){
        return;
    }
    int index = findMin();
    BinomialQueue newQueue;
    BinomialNode *tempNode;
    if(currentSize > 1){
        tempNode = theTrees[index]->firstChild;
    }
    for(int i = index - 1; i >= 0; i--){
        newQueue.theTrees[i] = tempNode;
        tempNode = tempNode->nextSibling;
        newQueue.theTrees[i]->nextSibling = nullptr;
    }
    currentSize -= (int)pow(2, index);
    newQueue.currentSize = (int)pow(2, index) - 1;
    delete theTrees[index];
    theTrees[index] = nullptr;
    merge(newQueue);
}

void BinomialQueue::printQueue(){
    for(int i = 0; i < MaxSize; i++){
        cout << "Tree " << i << ":";
        printQueue(theTrees[i]);
        cout << endl;
    }
}

void BinomialQueue::printQueue(BinomialNode *q){
    if(nullptr == q){
        return;
    }
    cout << q->key << " ";
    printQueue(q->firstChild);
    printQueue(q->nextSibling);
}

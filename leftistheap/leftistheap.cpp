#include "leftistheap.h"
#include <iostream>
using namespace std;

int min(int a, int b){
    return a < b ? a : b;
}

LeftistHeap::LeftistHeap()
    :root(nullptr){}

void LeftistHeap::merge(LeftistHeap &rhs){
    if(this != &rhs){
        root = merge(root, rhs.root);
        rhs.root = nullptr;
    }
}

void LeftistHeap::insert(int val){
    LeftistNode *newNode = new LeftistNode(val, nullptr, nullptr, 0);
    root = merge(root, newNode);
}

void LeftistHeap::deleteMin(){
    if(nullptr == root){//如果堆为空则直接返回，堆的删除操作一定要判断是否为空堆，
                        //当然也可以从指针解引用前一定要判断是否为nullptr来解释
        return;
    }
    LeftistNode *minNode = root;
    root = merge(root->left, root->right);
    delete minNode;
}

void LeftistHeap::printHeap() const{
    cout << "preorder:" << endl;
    preorder(root);
    cout << endl;
    cout << "inorder:" << endl;
    inorder(root);
    cout << endl;
}

void LeftistHeap::swapChild(LeftistNode *t){
    LeftistNode *tempNode = t->left;
    t->left = t->right;
    t->right = tempNode;
}

int LeftistHeap::getNpl(const LeftistNode *t) const{
    if(nullptr == t){
        return -1;
    }
    return t->npl;
}

LeftistHeap::LeftistNode* LeftistHeap::merge(LeftistNode *lhs, LeftistNode *rhs){
    LeftistNode *fixNode;

    if(lhs == nullptr){
        return rhs;
    }
    else if(rhs == nullptr){
        return lhs;
    }

    if(lhs->key < rhs->key){
        lhs->right = merge(lhs->right, rhs);
        fixNode = lhs;
    }
    //遇到相等的情况就直接继续插入，因为这是递归处理，
    //所以直接继续插入，插入的过程不会像非递归处理时那样停留在原地陷入死循环，
    //而是会继续搜索下去，并且最终一定能遇上nullptr节点并插入
    else{
        rhs->right = merge(lhs, rhs->right);
        fixNode = rhs;
    }

    if(getNpl(fixNode->left) < getNpl(fixNode->right)){
        swapChild(fixNode);
    }
    fixNode->npl = getNpl(fixNode->right) + 1;
    return fixNode;
}

void LeftistHeap::preorder(const LeftistNode *t) const{
    if(nullptr == t){
        return;
    }
    cout << t->key << " ";
    preorder(t->left);
    preorder(t->right);
}

void LeftistHeap::inorder(const LeftistNode *t) const{
    if(nullptr == t){
        return;
    }
    inorder(t->left);
    cout << t->key << " ";
    inorder(t->right);
}

#include "avltree.h"
#include <iostream>
using namespace std;

/*
 * 如果不想在函数内部改变形式参数或不想通过函数引用或指针的返回值改变返回值所指向或引用的变量，
 * 才将形式参数或返回值声明为const类型
 */

/*
 * 可以通过将函数形式参数声明为引用类型来实现在函数中对传入实际参数的改变，
 * 比如将一个一级指针声明为引用类型，就不需要传入一个二级指针来实现原来对一级指针进行改变的需求，
 * 但是如果将一个形式参数声明为引用类型就需要注意传入的实际参数不能为const类型，
 * 因为形式参数为引用类型的函数可以在函数内部实现对实际参数的改变，
 * 而不像C语言中的形式参数那样只能改变传入实际参数之外的变量而不能实现对实际参数的改变
 */

int max(const int a, const int b){
    return a > b ? a : b;
}

AVLTree::AVLTree()
    :root(nullptr){}

AVLTree::~AVLTree(){
    makeEmpty(root);
}

//将除指针和引用类型的返回值声明为const将不会起到任何作用，因为函数的返回值本来就是右值
//而将指针或引用类型的返回值声明为const可以使不能通过返回值来改变指向或引用的对象
//如果将返回值的指针或引用声明为const则需要注意不能赋值给普通的指针或引用类型的变量
int AVLTree::findMax() const{
    AVLNode* tempNode = findMax(root);
    if(nullptr == tempNode){
        return -1;
    }
    return tempNode->key;
}

int AVLTree::findMin() const{
    AVLNode* tempNode = findMin(root);//注意不能将一个指向常量指针赋值给一个普通指针
    if(nullptr == tempNode){
        return -1;
    }
    return tempNode->key;
}

void AVLTree::insertNode(const int val){
    insertNode(val, root);
}

void AVLTree::deleteNode(const int val){
    deleteNode(val, root);
}

void AVLTree::printTree(){
    printTree(root);
    cout << endl;
}

const AVLTree& AVLTree::operator =(const AVLTree &rhs){
    if(this != &rhs){//因为this实际上是一个AVLTree*类型的指针，所以需要&操作符
        makeEmpty(root);
        root = clone(rhs.root);
    }
    return *this;
}

void AVLTree::leftLeftRotate(AVLNode *&t){
    AVLNode *tempNode = t->left;

    t->left = tempNode->right;
    tempNode->right = t;
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;
    t = tempNode;
    tempNode->height = max(getHeight(tempNode->left), getHeight(tempNode->right)) + 1;
}

void AVLTree::leftRightRotate(AVLNode *&t){
    rightRightRotate(t->left);
    leftLeftRotate(t);
}

void AVLTree::rightLeftRotate(AVLNode *&t){
    leftLeftRotate(t->right);
    rightRightRotate(t);
}

void AVLTree::rightRightRotate(AVLNode *&t){
    AVLNode *tempNode = t->right;

    t->right = tempNode->left;
    tempNode->left = t;
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;
    t = tempNode;
    tempNode->height = max(getHeight(tempNode->left), getHeight(tempNode->right)) + 1;
}

AVLTree::AVLNode* AVLTree::findMin(AVLNode *t) const{
    if(nullptr == t){
        return nullptr;
    }

    while(nullptr != t->left){
        t = t->left;
    }

    return t;
}

AVLTree::AVLNode* AVLTree::findMax(AVLNode *t) const{
    if(nullptr == t){
        return nullptr;
    }

    while(nullptr != t->right){
        t = t->right;
    }

    return t;
}

int AVLTree::getHeight(AVLNode *t) const{
    if(nullptr == t){
        return -1;
    }
    return t->height;
}
/*
 * 插入或删除操作仅仅会改变从被插入或删除节点至根节点路径上的节点height，
 * 而对其它节点的height则没有影响（因为这里的height是从当前节点到叶节点的最长路径距离）
 * 所以仅仅需要调整这些路径上的节点即可，
 * 而因为是自底向上进行调整，并且其它节点的height均不变（有效），
 * 所以只需要根据每个节点的两个子节点进行调整即可，提升效率
 */
void AVLTree::insertNode(int val, AVLNode *&t){
    if(nullptr == t){
        t = new AVLNode(val, nullptr, nullptr);
    }
    else if(t->key > val){
        insertNode(val, t->left);
        if(getHeight(t->left) - getHeight(t->right) > 1){
            if(t->left->key > val){
                leftLeftRotate(t);
            }
            else{
                leftRightRotate(t);
            }
        }
    }
    else if(t->key < val){
        insertNode(val, t->right);
        if(getHeight(t->right) - getHeight(t->left) > 1){
            if(t->right->key > val){
                rightLeftRotate(t);
            }
            else{
                rightRightRotate(t);
            }
        }
    }
    else{
        ;
    }
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;
}

void AVLTree::deleteNode(int val, AVLNode *&t){
    if(nullptr == t){
        return;
    }
    if(t->key > val){
        deleteNode(val, t->left);
        if(getHeight(t->right) - getHeight(t->left) > 1){
            if(getHeight(t->right->left) > getHeight(t->right->right)){
                rightLeftRotate(t);
            }
            else{
                rightRightRotate(t);
            }
        }
    }
    else if(t->key < val){
        deleteNode(val, t->right);
        if(getHeight(t->left) - getHeight(t->right) > 1){
            if(getHeight(t->left->left) > getHeight(t->left->right)){
                leftLeftRotate(t);
            }
            else{
                leftLeftRotate(t);
            }
        }
    }
    else if(nullptr != t->left && nullptr != t->right){
        t->key = findMin(t->right)->key;
        deleteNode(t->key, t->right);
        if(getHeight(t->left) - getHeight(t->right) > 1){
            if(getHeight(t->left->left) > getHeight(t->left->right)){
                leftLeftRotate(t);
            }
            else{
                leftLeftRotate(t);
            }
        }
    }
    else{
        AVLNode *oldNode = t;
        if(nullptr == t->left){
            t = t->right;
        }
        else{
            t = t->left;
        }
        delete oldNode;
        return;//这里必须结束函数返回，因为这里可能出现使得t变为nullptr的情况
                //如果结束函数返回，那么接下来将可能出现对nullptr解引用的情况
    }
    t->height = max(getHeight(t->left), getHeight(t->right)) + 1;//注意这里的+1
}

void AVLTree::printTree(AVLNode *t) const{
    if(nullptr == t){
        return;
    }
    printTree(t->left);
    cout << t->key << " ";
    printTree(t->right);
}

void AVLTree::makeEmpty(AVLNode *&t){
    if(nullptr != t){
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}
/*
 * clone函数的形式参数不能为AVLNode *&t，不能为一个引用类型
 * 原因：
 * clone函数在operator=函数中被调用，operator=函数的形式参数是const AVLTree &rhs，
 * 意味着不能在operator=函数内部不能通过rhs来修改rhs中的成员变量root
 * （root在operator=函数中的类型为AVLNode *const，也就是一个指针常量），
 * 而如果clone函数的形式参数为一个引用类型，那么将rhs.root传入clone函数后，
 * 在clone函数中对clone函数形式参数的任何操作都相当于是对传入实际参数（即rhs.root）的操作，
 * 在clone函数调用结束后，对实际参数产生的影响依然存在，即一个const类型的rhs对象中的成员变量root可能会被改变
 * 而如果clone函数的形式参数不是一个引用类型，那么由于参数传递是值传递，仅仅只是实际参数值的拷贝
 * 在clone函数中对clone函数形式参数的任何操作都不会影响到传入的实际参数
 */
AVLTree::AVLNode* AVLTree::clone(AVLNode *t){
    if(nullptr == t){
        return nullptr;
    }
    return new AVLNode(t->key, clone(t->left), clone(t->right), t->height);
}

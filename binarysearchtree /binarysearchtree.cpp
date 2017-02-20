#include "binarysearchtree.h"

BinarySearchTree::~BinarySearchTree(){
    MakeEmpty();
}

const int BinarySearchTree::FindMax() const{
    BinaryNode* tempNode = FindMax(root);

    if(nullptr == tempNode){
        cout << "The binary search tree is empty now" << endl;
        return -1;
    }
    return tempNode->key;
}

BinarySearchTree::BinaryNode* BinarySearchTree::FindMax(BinarySearchTree::BinaryNode *t) const{
    if(true == IsEmpty()){
        return nullptr;
    }

    while(nullptr != t->right){
        t = t->right;
    }

    return t;
}

const int BinarySearchTree::FindMin() const{
    BinaryNode* tempNode = FindMin(root);

    if(nullptr == tempNode){
        cout << "The binary search tree is empty now" << endl;
        return -1;
    }
    return tempNode->key;
}
//函数名前的类名::实际上是指这个函数是该类的成员函数，并不包括返回值的类型，如果返回值的类型为该类中声明的自定义类型
//那么同样需要在返回值类型前加类名::
BinarySearchTree::BinaryNode* BinarySearchTree::FindMin(BinaryNode *t) const{
    if(true == IsEmpty()){
        return nullptr;
    }

    while(nullptr != t->left){
        t = t->left;
    }

    return t;
}

bool BinarySearchTree::Contains(const int &val) const{
    return Contains(root, val);
}

bool BinarySearchTree::Contains(BinaryNode *t, const int &val) const{
    if(true == IsEmpty()){
        return false;
    }

    while(nullptr != t){
        if(t->key > val){
            t = t->left;
        }
        else if(t->key < val){
            t = t->right;
        }
        else{
            return true;
        }
    }

    return false;
}

bool BinarySearchTree::IsEmpty() const{
    if(nullptr == root){
        return true;
    }
    return false;
}

void BinarySearchTree::PrintTree() const{
    PrintTree(root);
}

void BinarySearchTree::PrintTree(BinaryNode *t) const{
    if(nullptr == t){
        return;
    }

    PrintTree(t->left);
    cout << t->key << " ";
    PrintTree(t->right);
}

void BinarySearchTree::Insert(const int & val){
    Insert(root, val);
}

void BinarySearchTree::Insert(BinaryNode *&t, const int &val){
    if(nullptr == t){//任何指针在进行解引用之前都应该考虑其是否为nullptr
        t = new BinaryNode(val, nullptr, nullptr);
    }
    else if(t->key > val){
        Insert(t->left, val);
    }
    else if(t->key < val){
        Insert(t->right, val);
    }
    else{
        ;//这里如果什么都不做的话，等同于非递归版本中处理重复插入时的return语句，
        //因为递归函数如果不进行递归调用就会返回
    }
}

void BinarySearchTree::Remove(const int &val){
    Remove(root, val);
}

void BinarySearchTree::Remove(BinaryNode *&t, const int &val){
    BinaryNode *tempNode;

    if(nullptr == t){
        return;
    }
    if(t->key > val){
        Remove(t->left, val);
    }
    else if(t->key < val){
        Remove(t->right, val);
    }
    else if(nullptr != t->left && nullptr != t->right){
        t->key = FindMin(t->right)->key;
        Remove(t->right, t->key);
    }
    else if(t->key == val){//删除节点后需要回收内存空间，并且只有单支节点或叶节点需要回收空间
        tempNode = t;
        if(nullptr == t->left && nullptr == t->right){
            t = nullptr;
        }
        else if(nullptr == t->left){
            t = t->right;
        }
        else if(nullptr == t->right){
            t = t->left;
        }
        delete tempNode;
    }
}

void BinarySearchTree::MakeEmpty(){
    MakeEmpty(root);
}

void BinarySearchTree::MakeEmpty(BinaryNode *&t){
    if(nullptr != t){//树结构应该多考虑递归
        MakeEmpty(t->left);
        MakeEmpty(t->right);
        delete t;
    }
    t = nullptr;//最终指针应该被赋值为nullptr，并且最终的root也应该被赋值为nullptr
}

BinarySearchTree::BinaryNode* BinarySearchTree::Clone(BinaryNode *t) const{
    if(nullptr == t){//递归出口
        return nullptr;
    }
    return new BinaryNode(t->key, Clone(t->left), Clone(t->right));//new 类型名，返回指向新的内存空间的指针
}

const BinarySearchTree & BinarySearchTree::operator=(const BinarySearchTree &rhs){
    if(this != &rhs){//必须有这个条件判断，否则如果赋值号两边是同一个对象，那么该对象将直接被删除而不是赋值给另一个对象
        MakeEmpty();
        root = Clone(rhs.root);//成员函数的代码中可以访问任何一个该类对象的所有成员变量，无论其是否为private属性
                                //只需要将该类某个对象的引用或指针作为参数传入函数即可
    }
    return *this;
}

/*
 * 这个程序的抽象逻辑是：
 * 将二叉查找树看作是具有一定特点的一个类，
 * 插入、查找、删除等等操作也是针对一整棵树来说的，因此类的成员函数就需要实现这些操作
 * 而一棵二叉查找树的数据看似有很多（每个节点都是）
 * 但实际上因为每个节点通过指针联系在一起，所以一个二叉查找树作为一个对象，只需要包含根节点的数据，
 * 就相当于是包含了这棵树的所有数据，可以对所有数据进行操作
 */

#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iostream>
using namespace std;

/*
 * 这里不希望外部可以直接访问使用类的私有成员变量，然而一些需要递归的函数却需要私有成员变量作为参数传入，
 * 因此就利用函数重载声明两个属性分别为private和public的成员函数
 * public的成员函数调用private的成员函数，同时在public的成员函数中将私有成员变量作为参数传入
 * 这样使得外部不能直接使用私有成员变量，但是却可以通过调用public的函数来实现需求
 */
class BinarySearchTree
{
public:
    BinarySearchTree()
        : root(nullptr) {}
    BinarySearchTree(const BinarySearchTree &rhs);
    ~BinarySearchTree();

    const int FindMax() const;
    const int FindMin() const;
    bool Contains(const int &val) const;
    bool IsEmpty() const;
    void PrintTree() const;
    void MakeEmpty();
    void Insert(const int &val);
    void Remove(const int &val);

    const BinarySearchTree & operator=(const BinarySearchTree &rhs);
private:
    //可以在类中嵌套声明类或结构，这里的结构不是类的成员变量，
    //这里类的成员变量只有root
    struct BinaryNode {
        int key;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const int &val, BinaryNode *lchild, BinaryNode *rchild)
            : key(val), left(lchild), right(rchild) {}
    };

    BinaryNode *root;

    BinaryNode* FindMax(BinaryNode *t) const;
    BinaryNode* FindMin(BinaryNode *t) const;
    bool Contains(BinaryNode *t, const int &val) const;
    void MakeEmpty(BinaryNode *&t);
    void PrintTree(BinaryNode *t) const;
    void Insert(BinaryNode *&t, const int &val);
    void Remove(BinaryNode *&t, const int &val);

    BinaryNode* Clone(BinaryNode *t) const;
};

#endif // BINARYSEARCHTREE_H

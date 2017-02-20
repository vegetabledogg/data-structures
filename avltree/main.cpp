#include <iostream>
#include "avltree.h"
using namespace std;

int main(){
    int val;
    AVLTree ATree, BTree;

    cout << "Enter the value to be inserted into ATree and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        ATree.insertNode(val);
    }
    cout << "Print the whole ATree:" << endl;
    ATree.printTree();
    cout << "Enter the value to be inserted into BTree and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        BTree.insertNode(val);
    }
    cout << "Print the whole BTree:" << endl;
    BTree.printTree();
    cout << "The max key in the ATree: " << ATree.findMax() << endl;
    cout << "The min key in the BTree: " << BTree.findMin() << endl;
    cout << "The height of the ATree: " << ATree.getHeight() << endl;
    cout << "Enter the value to be deleted from ATree and end with -1: " << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        ATree.deleteNode(val);
    }
    cout << "Print the whole ATree:" << endl;
    ATree.printTree();
    ATree = BTree;
    cout << "Print the whole ATree after assignment:" << endl;
    ATree.printTree();
}

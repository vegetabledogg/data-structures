#include "binarysearchtree.h"

int main(){
    int val;
    BinarySearchTree ATree, BTree;

    cout << "Enter the value to be inserted and end with -1: " << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        ATree.Insert(val);
        BTree.Insert(val + 1);
    }
    cout << "The maximum number of the Atree is " << ATree.FindMax() << endl;
    cout << "The minimum number of the Atree is " << ATree.FindMin() << endl;
    cout << "Print the whole Atree: " << endl;
    ATree.PrintTree();
    cout << endl;
    cout << "Enter the value to be searched and end with -1: " << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        cout << ATree.Contains(val) << endl;
    }
    cout << "Enter the value to be removed and end with -1: " << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        ATree.Remove(val);
    }
    cout << "Print the whole Atree: " << endl;
    ATree.PrintTree();
    cout << endl;
    ATree = BTree;
    cout << "Print the whole Atree: " << endl;
    ATree.PrintTree();
    cout << endl;

    return 0;
}

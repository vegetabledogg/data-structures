#include "splaytree.h"

int main(){
    int val;
    SplayTree ATree, BTree;

    cout << "Enter the value to be inserted into ATree and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        ATree.insert(val);
    }
    cout << "Print the whole ATree:" << endl;
    ATree.printTree();
    cout << "The max key in the ATree: " << ATree.findMax() << endl;
    cout << "Enter the value to be deleted from ATree and end with -1: " << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        ATree.remove(val);
    }
    cout << "Print the whole ATree:" << endl;
    ATree.printTree();
}

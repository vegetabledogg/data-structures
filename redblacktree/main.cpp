#include <iostream>
#include "redblacktree.h"

using namespace std;

int main(){
    RedBlackTree T;
    int val;

    cout << "Enter the value to be inserted and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        T.insert(val);
    }
    T.printTree();
    cout << "Enter the value to be deleted and end with '-1':" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        T.remove(val);
    }
    T.printTree();

    return 0;
}

#include <iostream>
#include "btree.h"
using namespace std;

int main(){
    BTree t;
    int val;

    cout << "Enter the value to be inserted and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        t.insert(val);
        t.printTree();
    }
    cout << "Enter the value to be deleted and end with '-1':" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        t.deleteKey(val);
        t.printTree();
    }

    return 0;
}

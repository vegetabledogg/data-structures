#include "binomialqueue.h"
#include <iostream>
using namespace std;

int main(){
    int val;
    BinomialQueue queue;

    cout << "Enter the keys to be inserted and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        queue.insert(val);
        queue.printQueue();
    }
    cout << "Enter the number of keys to be deleted:" << endl;
    cin >> val;
    while(val--){
        queue.deleteMin();
        queue.printQueue();
    }
}

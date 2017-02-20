#include <iostream>
#include "leftistheap.h"
using namespace std;

int main(){
    int val;
    LeftistHeap h;

    cout << "Enter the val to be inserted into the heap and end with -1:" << endl;
    while(1){
        cin >> val;
        if(-1 == val){
            break;
        }
        h.insert(val);
    }
    h.printHeap();
    cout << "Enter the number of nodes to be delete from the heap:" << endl;
    cin >> val;
    while(val--){
        h.deleteMin();
        h.printHeap();
    }
}

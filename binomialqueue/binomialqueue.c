#include <stdio.h>
#include <stdlib.h>

#define ElementType int
#define MaxTree 8
#define Capacity 511

typedef struct BinomialNode *PtrBonimialNode;
struct BinomialNode{
    ElementType key;
    PtrBonimialNode firstChild;
    PtrBonimialNode nextSibling;
};

typedef struct BinomialQueue *PtrBonimialQueue;
struct BinomialQueue{
    int currentSize;
    PtrBonimialNode theTrees[MaxTree];
};

int myPow(int base, int exp);
PtrBonimialNode allocateNode(ElementType val);
int findMin(PtrBonimialNode theTrees[]);
PtrBonimialQueue init();
PtrBonimialNode combineTree(PtrBonimialNode lt, PtrBonimialNode rt);
void merge(PtrBonimialQueue lhs, PtrBonimialQueue rhs);
void insert(PtrBonimialQueue queue, ElementType val);
void deleteMin(PtrBonimialQueue queue);
void print(PtrBonimialQueue queue);
void preTraversal(PtrBonimialNode t);

int main(){
    PtrBonimialQueue queue = init();
    int val;
    
    printf("Enter the values to be inserted to the queue and end with -1\n");
    while(1){
        scanf("%d", &val);
        if(-1 == val){
            break;
        }
        insert(queue, val);
    }
    print(queue);
    printf("Enter the number of values to be deleted\n");
    scanf("%d", &val);
    while(val--){
        deleteMin(queue);
    }
    print(queue);
    return 0;
}

int myPow(int base, int exp){
    int i;
    
    if(0 == base && 0 == exp){
        printf("ERROR\n");
        return 0;
    }
    else if(0 == exp){
        return 1;
    }
    else{
        for(i = 0; i < exp - 1; i++){
            base *= base;
        }
        return base;
    }
}

PtrBonimialNode allocateNode(ElementType val){
    PtrBonimialNode newNode = (PtrBonimialNode)malloc(sizeof(struct BinomialNode));
    newNode->firstChild = newNode->nextSibling = NULL;
    newNode->key = val;
    return newNode;
}

int findMin(PtrBonimialNode theTrees[]){
    int i, min = 100000, index = 0;
    
    for(i = 0; i < MaxTree; i++){
        if(NULL != theTrees[i] && theTrees[i]->key < min){
            min = theTrees[i]->key;
            index = i;
        }
    }
    return index;
}

PtrBonimialQueue init(){
    int i;
    PtrBonimialQueue newQueue = (PtrBonimialQueue)malloc(sizeof(struct BinomialQueue));
    newQueue->currentSize = 0;
    for(i = 0; i < MaxTree; i++){
        newQueue->theTrees[i] = NULL;
    }
    return newQueue;
}

PtrBonimialNode combineTree(PtrBonimialNode lt, PtrBonimialNode rt){
    if(lt->key < rt->key){
        rt->nextSibling = lt->firstChild;
        lt->firstChild = rt;
        return lt;
    }
    else{
        return combineTree(rt, lt);
    }
}

void merge(PtrBonimialQueue lhs, PtrBonimialQueue rhs){
    int i;
    PtrBonimialNode carry = NULL, tempNode;
    
    lhs->currentSize += rhs->currentSize;
    if(lhs->currentSize > Capacity){
        lhs->currentSize -= rhs->currentSize;
        printf("Out of capacity, can not merge!\n");
        return;
    }
    for(i = 0; i < MaxTree; i++){
        if(NULL == carry){
            if(NULL == rhs->theTrees[i]){
                ;
            }
            else if(NULL == lhs->theTrees[i]){
                lhs->theTrees[i] = rhs->theTrees[i];
            }
            else{
                carry = combineTree(lhs->theTrees[i], rhs->theTrees[i]);
                lhs->theTrees[i] = NULL;
            }
        }
        else{
            if(NULL == rhs->theTrees[i] && NULL == lhs->theTrees[i]){
                lhs->theTrees[i] = carry;
                carry = NULL;
            }
            else if(NULL == lhs->theTrees[i]){
                carry = combineTree(carry, rhs->theTrees[i]);
            }
            else if(NULL == rhs->theTrees[i]){
                carry = combineTree(carry, lhs->theTrees[i]);
                lhs->theTrees[i] = NULL;
            }
            else{
                tempNode = carry;
                carry = combineTree(lhs->theTrees[i], rhs->theTrees[i]);
                lhs->theTrees[i] = tempNode;
            }
        }
    }
}

void insert(PtrBonimialQueue queue, ElementType val){
    PtrBonimialQueue newQueue = init();
    newQueue->theTrees[0] = allocateNode(val);
    newQueue->currentSize = 1;
    merge(queue, newQueue);
}

void deleteMin(PtrBonimialQueue queue){
    int i, index = findMin(queue->theTrees);
    PtrBonimialNode tempNode, minNode;
    PtrBonimialQueue newQueue = init();
    
    if(0 == queue->currentSize){
    	return;
    }
    minNode = queue->theTrees[index];
    newQueue->currentSize = myPow(2, index) - 1;
    tempNode = minNode->firstChild;
    queue->theTrees[index] = NULL;
    queue->currentSize -= myPow(2, index);
    for(i = index - 1; i >= 0; i--){
         newQueue->theTrees[i] = tempNode;
         tempNode = tempNode->nextSibling;
         newQueue->theTrees[i]->nextSibling = NULL;
    }
    merge(queue, newQueue);
}

void print(PtrBonimialQueue queue){  
    int i;
    
    for(i = 0; i < MaxTree; i++){ 
        printf("Tree %d:\n",i); 
        preTraversal(queue->theTrees[i]);
        printf("\n");
    }   
    return;  
}  

void preTraversal(PtrBonimialNode t){
    if(NULL == t){
        return;
    }
    
    printf("%d ", t->key);  
    preTraversal(t->firstChild);
    preTraversal(t->nextSibling);
}  

//test case:
//insert: 12,1,9,2,0,11,7,19,4,15,18,5,14,13,10,16,6,3,8,17,20,21,23
//delete: 16,18,20,19,0,5,2
#include <stdio.h>
#include <stdlib.h>

#define MinDegree 3

typedef int ElementType;
typedef int* PtrElementType;

typedef enum Bool BoolType;
enum Bool{
    False = 0,
    True = 1
};

typedef struct BplusNode *PtrBplusNode;
typedef struct BplusNode BplusNode;
struct BplusNode{
    int keyNum;
    BoolType isLeaf;
    PtrElementType key;
    PtrBplusNode *child;
    PtrBplusNode next;
};

typedef struct BplusTree *PtrBplusTree;
struct BplusTree{
    PtrBplusNode root;
};

PtrBplusNode allocateNode(BoolType IsLeaf);
void split(PtrBplusNode splitNodeP, int ChildIndex);
void insertNonFull(PtrBplusNode CurrentNode, ElementType Val);
void insert(PtrBplusTree t, ElementType val);
void merge(PtrBplusTree T, PtrBplusNode CurrentNode, int LeftIndex, int RightIndex);
void deleteFix(PtrBplusTree T, PtrBplusNode CurrentNode, ElementType Val);
void shiftKey(PtrElementType Key, BoolType Direction, int Begin, int End);
void shiftChild(PtrBplusNode *Child, BoolType Direction, int Begin, int End);
int getIndex(PtrElementType Key, int Size, ElementType Val);
void printTree(PtrBplusNode Root);
void BpCreateTree(PtrBplusTree T);

int main(){
    int val;
    PtrBplusTree t = (PtrBplusTree)malloc(sizeof(struct BplusTree));
    
    t->root = allocateNode(True);
    printf("Enter the keys to be inserted and end with -1:\n");
    while(1){
        scanf("%d", &val);
        if(-1 == val){
            break;
        }
        insert(t, val);
        printTree(t->root);
    }
    printf("Enter the keys to be deleted and end with -1:\n");
    while(1){
        scanf("%d", &val);
        if(-1 == val){
            break;
        }
        deleteFix(t, t->root, val);
        printTree(t->root);
    }
    
    return 0;
}

PtrBplusNode allocateNode(BoolType isLeaf){
    int i;
    PtrBplusNode newNode = (PtrBplusNode)malloc(sizeof(BplusNode));
    
    newNode->keyNum = 0;
    if(True == isLeaf){
        newNode->isLeaf = True;
    }
    else{
        newNode->isLeaf = False;
    }
    newNode->key = (PtrElementType)malloc(sizeof(ElementType) * (MinDegree * 2 - 1));
    newNode->child =(PtrBplusNode*)malloc(sizeof(PtrBplusNode) * MinDegree * 2);
    for(i = 0; i < MinDegree * 2; i++){
        newNode->child[i] = NULL;
    }
    newNode->next = NULL;
    
    return newNode;
}

void insert(PtrBplusTree t, ElementType val){
    PtrBplusNode newNode;
    
    if(MinDegree * 2 - 1 == t->root->keyNum){
        newNode = allocateNode(False);
        newNode->child[0] = t->root;
        t->root = newNode;
    }
    insertNonFull(t->root, val);
}

void insertNonFull(PtrBplusNode node, ElementType val){
    int index = getIndex(node->key, node->keyNum, val);
    
    if(True == node->isLeaf){
        shiftKey(node->key, True, index, node->keyNum - 1);
        node->key[index] = val;
        (node->keyNum)++;
    }
    else{
        if(MinDegree * 2 - 1 == node->child[index]->keyNum){
            split(node, index);
            if(node->key[index] < val){
                index++;
            }
        }
        insertNonFull(node->child[index], val);
    }
}

void split(PtrBplusNode node, int childIndex){
    int i;
    PtrBplusNode newNode, subNode = node->child[childIndex];
    
    if(True == subNode->isLeaf){
        newNode = allocateNode(True);
        for(i = 0; i < MinDegree - 1; i++){
            newNode->key[i] = subNode->key[i + MinDegree];
        }
        newNode->keyNum = MinDegree - 1;
        subNode->keyNum = MinDegree;
        newNode->next = subNode->next;
        subNode->next = newNode;
        
        shiftKey(node->key, True, childIndex, node->keyNum - 1);
        shiftChild(node->child, True, childIndex + 1, node->keyNum);
        node->key[childIndex] = subNode->key[MinDegree - 1];
        node->child[childIndex + 1] = newNode;
        (node->keyNum)++;
    }
    else{
        newNode = allocateNode(False);
        for(i = 0; i < MinDegree - 1; i++){
            newNode->key[i] = subNode->key[i + MinDegree];
        }
        for(i = 0; i < MinDegree; i++){
            newNode->child[i] = subNode->child[i + MinDegree];
        }
        newNode->keyNum = subNode->keyNum = MinDegree - 1;
        
        shiftKey(node->key, True, childIndex, node->keyNum - 1);
        shiftChild(node->child, True, childIndex + 1, node->keyNum);
        node->key[childIndex] = subNode->key[MinDegree - 1];
        node->child[childIndex + 1] = newNode;
        (node->keyNum)++;
    }
}

void shiftKey(PtrElementType key, BoolType direction, int begin, int end){
    int i;
    
    if(True == direction){
        for(i = end; i >= begin; i--){
            key[i + 1] = key[i];
        }
    }
    else{
        for(i = begin; i <= end; i++){
            key[i - 1] = key[i];
        }
    }
}

void shiftChild(PtrBplusNode *child, BoolType direction, int begin, int end){
    int i;
    
    if(True == direction){
        for(i = end; i >= begin; i--){
            child[i + 1] = child[i];
        }
    }
    else{
        for(i = begin; i <= end; i++){
            child[i - 1] = child[i];
        }
    }
}

int getIndex(PtrElementType key, int size, ElementType val){
    int i;
    
    for(i = 0; i < size; i++){
        if(key[i] >= val){
            break;
        }
    }
    
    return i;
}

void deleteFix(PtrBplusTree t, PtrBplusNode currentNode, ElementType val){
    int index = getIndex(currentNode->key, currentNode->keyNum, val);
    PtrBplusNode leftSibling = NULL, subNode = NULL, rightSibling = NULL;
    
    if(True == currentNode->isLeaf){
        if(index < currentNode->keyNum && currentNode->key[index] == val){
            shiftKey(currentNode->key, False, index + 1, currentNode->keyNum - 1);
            currentNode->keyNum -= 1;
        }
    }
    else{
        if(index > 0){
            leftSibling = currentNode->child[index - 1];
        }
        subNode = currentNode->child[index];
        if(index < currentNode->keyNum){
            rightSibling = currentNode->child[index + 1];
        }
        if(index < currentNode->keyNum && currentNode->key[index] == val){
            if(subNode->keyNum >= MinDegree){
                if(True == subNode->isLeaf){
                    currentNode->key[index] = subNode->key[subNode->keyNum - 2];
                    subNode->keyNum -= 1;
                }
                else{
                    currentNode->key[index] = subNode->key[subNode->keyNum - 1];
                    deleteFix(t, subNode, currentNode->key[index]);
                }
            }
            else if(rightSibling->keyNum >= MinDegree){
                currentNode->key[index] = rightSibling->key[0];
                if(True == subNode->isLeaf){
                    subNode->key[subNode->keyNum - 1] = currentNode->key[index];
                }
                deleteFix(t, rightSibling, currentNode->key[index]);
            }
            else{
                merge(t, currentNode, index, index + 1);
                deleteFix(t, subNode, val);
            }
        }
        else{
            if(subNode->keyNum >= MinDegree){
                deleteFix(t, subNode, val);
            }
            else if(NULL != leftSibling && leftSibling->keyNum >= MinDegree){
                shiftKey(subNode->key, True, 0, subNode->keyNum - 1);
                shiftChild(subNode->child, True, 0, subNode->keyNum);
                subNode->key[0] = currentNode->key[index - 1];
                subNode->child[0] = leftSibling->child[leftSibling->keyNum];
                subNode->keyNum += 1;
                if(True == subNode->isLeaf){
                    currentNode->key[index - 1] = leftSibling->key[leftSibling->keyNum - 2];
                }
                else{
                    currentNode->key[index - 1] = leftSibling->key[leftSibling->keyNum - 1];
                }
                leftSibling->keyNum -= 1;
                deleteFix(t, subNode, val);
            }
            else if(NULL != rightSibling && rightSibling->keyNum >= MinDegree){
                if(True == subNode->isLeaf){
                    subNode->key[subNode->keyNum] = rightSibling->key[0];
                }
                else{
                    subNode->key[subNode->keyNum] = currentNode->key[index];
                }
                subNode->child[subNode->keyNum + 1] = rightSibling->child[0];
                subNode->keyNum += 1;
                currentNode->key[index] = rightSibling->key[0];
                shiftKey(rightSibling->key, False, 1, rightSibling->keyNum - 1);
                shiftChild(rightSibling->child, False, 1, rightSibling->keyNum);
                rightSibling->keyNum -= 1;
                deleteFix(t, subNode, val);
            }
            else{
                if(NULL != leftSibling){
                    merge(t, currentNode, index - 1, index);
                    deleteFix(t, leftSibling, val);
                }
                else{
                    merge(t, currentNode, index, index + 1);
                    deleteFix(t, subNode, val);
                }
            }
        }
    }
}

void merge(PtrBplusTree t, PtrBplusNode currentNode, int leftIndex, int rightIndex){
    int i;
    PtrBplusNode leftNode = currentNode->child[leftIndex];
    PtrBplusNode rightNode = currentNode->child[rightIndex];
    
    if(True == leftNode->isLeaf){
        for(i = 0; i < MinDegree - 1; i++){
            leftNode->key[i + MinDegree - 1] = rightNode->key[i];
        }
        leftNode->keyNum = MinDegree * 2 - 2;
        leftNode->next = rightNode->next;
    }
    else{
        for(i = 0; i < MinDegree - 1; i++){
            leftNode->key[i + MinDegree] = rightNode->key[i];
        }
        for(i = 0; i < MinDegree; i++){
            leftNode->key[i + MinDegree] = rightNode->key[i];
        }
        leftNode->key[MinDegree - 1] = currentNode->key[leftIndex];
        leftNode->keyNum = MinDegree * 2 - 1;
    }
    
    shiftKey(currentNode->key, False, leftIndex + 1, currentNode->keyNum - 1);
    shiftChild(currentNode->child, False, rightIndex + 1, currentNode->keyNum);
    (currentNode->keyNum)--;
    
    if(currentNode == t->root && 0 == currentNode->keyNum){
        t->root = leftNode;
    }
}

void printTree(PtrBplusNode root){
    int i;
    
    if(NULL == root){
        return;
    }
    
    putchar('[');
    for(i = 0; i < root->keyNum; i++){
        printf("%d", root->key[i]);
        if(i != root->keyNum - 1){
            putchar(' ');
        }
    }
    putchar(']');
    printf("%d ", root->isLeaf);
    printf("%d", root->keyNum);
    putchar('\n');
    
    for(i = 0; i <= root->keyNum; i++){
        printTree(root->child[i]);
    }
}

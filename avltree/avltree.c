/*
 The main function is used as a test program,
 and one possible test sample is as follows:
 20 10 5 8 7 30 40 29 25 15 18 17 -1
 After all the insertions, the tree is like:
 18
 /  \
 /    \
 /      \
 10      25
 /  \    /  \
 7   15  20  30
 / \    \    /  \
 5   8   17  29  40
 */
#include <stdio.h>
#include <stdlib.h>

#define ElementType int
#define MaxNum 32767

typedef struct AVLNode *PtrAVLNode;
struct AVLNode{
    ElementType key;
    int height;
    PtrAVLNode left;
    PtrAVLNode right;
};

int max(int a, int b);
PtrAVLNode allocateNode(ElementType Val);
int height(PtrAVLNode t);
PtrAVLNode findMin(PtrAVLNode t);
void levelorder(PtrAVLNode t);
PtrAVLNode leftLeftRotate(PtrAVLNode t);
PtrAVLNode leftRightRotate(PtrAVLNode t);
PtrAVLNode rightLeftRotate(PtrAVLNode t);
PtrAVLNode rightRightRotate(PtrAVLNode t);
PtrAVLNode insert(PtrAVLNode t, ElementType val);
PtrAVLNode delete(PtrAVLNode t, ElementType val);

int main() {
    PtrAVLNode t = NULL;
    int val;
    
    printf("Enter the node to be inserted and end with '-1':");
    while(scanf("%d", &val)){
        if(val == -1)
            break;
        t = insert(t, val);
    }
    printf("Level order traversal:");
    levelorder(t);
    printf("Enter the node to be deleted:");
    scanf("%d", &val);
    t = delete(t, val);
    printf("Level order traversal:");
    levelorder(t);
    return 0;
}

int max(int a, int b){
    return a > b ? a : b;
}

PtrAVLNode allocateNode(ElementType val){
    PtrAVLNode newNode = (PtrAVLNode)malloc(sizeof(struct AVLNode));
    
    newNode->key = val;
    newNode->height = 0;
    newNode->left = newNode->right = NULL;
    
    return newNode;
}

int height(PtrAVLNode t){
    if(NULL == t){
        return -1;
    }
    return t->height;
}

PtrAVLNode findMin(PtrAVLNode t){
    while(NULL != t->left){
        t = t->left;
    }
    return t;
}

void levelorder(PtrAVLNode t){
    PtrAVLNode queue[MaxNum];
    PtrAVLNode tempNode;
    int head = 0, rear = -1;
    
    if(NULL != t){
        queue[++rear] = t;
    }
    while(head <= rear){
        tempNode = queue[head++];
        printf("%d ", tempNode->key);
        if(NULL != tempNode->left){
            queue[++rear] = tempNode->left;
        }
        if(NULL != tempNode->right){
            queue[++rear] = tempNode->right;
        }
    }
}

PtrAVLNode leftLeftRotate(PtrAVLNode t){
    PtrAVLNode tempNode = t->left;
    
    t->left = tempNode->right;
    tempNode->right = t;
    t->height = max(height(t->left), height(t->right)) + 1;
    tempNode->height = max(height(tempNode->left), height(tempNode->right)) + 1;
    
    return tempNode;
}

PtrAVLNode leftRightRotate(PtrAVLNode t){
    t->left = rightRightRotate(t->left);
    return leftLeftRotate(t);
}

PtrAVLNode rightLeftRotate(PtrAVLNode t){
    t->right = leftLeftRotate(t->right);
    return rightRightRotate(t);
}

PtrAVLNode rightRightRotate(PtrAVLNode t){
    PtrAVLNode tempNode = t->right;
    
    t->right = tempNode->left;
    tempNode->left = t;
    t->height = max(height(t->left), height(t->right)) + 1;
    tempNode->height = max(height(tempNode->left), height(tempNode->right)) + 1;
    
    return tempNode;
}

PtrAVLNode insert(PtrAVLNode t, ElementType val){
    int leftHeight, rightHeight;
    
    if(NULL == t){
        t = allocateNode(val);
        return t;
    }
    
    if(val < t->key){
        t->left = insert(t->left, val);
    }
    else if(val > t->key){
        t->right = insert(t->right, val);
    }
    else{
        return t;
    }
    
    leftHeight = height(t->left);
    rightHeight = height(t->right);
    if(leftHeight - rightHeight > 1){
        if(val > t->left->key){
            return leftRightRotate(t);
        }
        else{
            return leftLeftRotate(t);
        }
    }
    else if(rightHeight - leftHeight > 1){
        if(val < t->right->key){
            return rightLeftRotate(t);
        }
        else{
            return rightRightRotate(t);
        }
    }
    
    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}

PtrAVLNode delete(PtrAVLNode t, ElementType val){
    PtrAVLNode tempNode;
    int leftHeight, rightHeight;
    
    if(NULL == t){
        return NULL;
    }
    
    if(val < t->key){
        t->left = delete(t->left, val);
    }
    else if(val > t->key){
        t->right = delete(t->right, val);
    }
    else{
        if(NULL == t->left){
            return t->right;
        }
        else if(NULL == t->right){
            return t->left;
        }
        else{
            tempNode = findMin(t->right);
            t->key = tempNode->key;
            t->right = delete(t->right, t->key);
        }
    }
    
    leftHeight = height(t->left);
    rightHeight = height(t->right);
    if(leftHeight - rightHeight > 1){
        if(height(t->left->left) >= height(t->left->right)){
            return leftLeftRotate(t);
        }
        else{
            return leftRightRotate(t);
        }
    }
    else if(rightHeight - leftHeight > 1){
        if(height(t->right->right) >= height(t->right->left)){
            return rightRightRotate(t);
        }
        else{
            return rightLeftRotate(t);
        }
    }
    
    t->height = max(height(t->left), height(t->right)) + 1;
    return t;
}

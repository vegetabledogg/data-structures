#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ElementType int

enum{RED, BLACK};

typedef struct RedBlackNode *PtrRedBlackNode;
struct RedBlackNode{
    ElementType key;
    PtrRedBlackNode left;
    PtrRedBlackNode right;
    PtrRedBlackNode parent;
    int color;
};

typedef struct RedBlackTree *PtrRedBlackTree;
struct RedBlackTree{
    PtrRedBlackNode root;
    PtrRedBlackNode nullNode;
};

PtrRedBlackNode allocateNode(PtrRedBlackTree t, int val);
PtrRedBlackTree init();
void printTree(PtrRedBlackTree t);
void inOrder(PtrRedBlackTree t, PtrRedBlackNode node);
void preOrder(PtrRedBlackTree t, PtrRedBlackNode node);
PtrRedBlackNode find(PtrRedBlackTree t, int val);
PtrRedBlackNode findMin(PtrRedBlackTree t, PtrRedBlackNode node);
void leftRotate(PtrRedBlackTree t, PtrRedBlackNode pivot);
void rightRotate(PtrRedBlackTree t, PtrRedBlackNode pivot);
void insertFixUp(PtrRedBlackTree t, PtrRedBlackNode fixNode);
void insert(PtrRedBlackTree t, int val);
void deleteFixUp(PtrRedBlackTree t, PtrRedBlackNode fixNode);
void deleteNode(PtrRedBlackTree t, int val);

int main(){
    PtrRedBlackTree t = init();
    ElementType times, val, count = 1;
    
    srand((unsigned int)time(NULL));
    printf("Enter the number of nodes to be inserted:");
    scanf("%d", &times);
    printf("Insert %d nodes into an empty Red Black Tree ramdomly:\n", times);
    while(times--){
        val = rand() % 100;
        insert(t, val);
        printf("Round: %d: insert %d\n", count++, val);
        printTree(t);
    }
    printf("Enter the node to be deleted and end with '-1':");
    while(scanf("%d", &val)){
        if(-1 == val){
            break;
        }
        deleteNode(t, val);
        printTree(t);
    }
}

PtrRedBlackNode allocateNode(PtrRedBlackTree t, int val){
    PtrRedBlackNode newNode = (PtrRedBlackNode)malloc(sizeof(struct RedBlackNode));
    newNode->key = val;
    newNode->left = newNode->right = newNode->parent = t->nullNode;
    newNode->color = RED;
    return newNode;
}

PtrRedBlackTree init(){
    PtrRedBlackTree t = (PtrRedBlackTree)malloc(sizeof(struct RedBlackTree));
    t->nullNode = (PtrRedBlackNode)malloc(sizeof(struct RedBlackNode));
    t->nullNode->key = -1;
    t->nullNode->left = t->nullNode->right = t->nullNode->parent = NULL;
    t->nullNode->color = BLACK;
    t->root = t->nullNode;
    return t;
}

void printTree(PtrRedBlackTree t){
    inOrder(t, t->root);
    putchar('\n');
    preOrder(t, t->root);
    putchar('\n');
}

void inOrder(PtrRedBlackTree t, PtrRedBlackNode node){
    if(t->nullNode == node){
        return;
    }
    inOrder(t, node->left);
    printf("%d-%d ", node->key, node->color);
    inOrder(t, node->right);
}

void preOrder(PtrRedBlackTree t, PtrRedBlackNode node){
    if(t->nullNode == node){
        return;
    }
    printf("%d-%d ", node->key, node->color);
    preOrder(t, node->left);
    preOrder(t, node->right);
}

PtrRedBlackNode find(PtrRedBlackTree t, int val){
    if(t->root == t->nullNode){
        return t->nullNode;
    }
    PtrRedBlackNode tempNode = t->root;
    while(tempNode != t->nullNode){
        if(tempNode->key > val){
            tempNode = tempNode->left;
        }
        else if(tempNode->key < val){
            tempNode = tempNode->right;
        }
        else{
            break;
        }
    }
    return tempNode;
}

PtrRedBlackNode findMin(PtrRedBlackTree t, PtrRedBlackNode node){
    while(t->nullNode != node->left){
        node = node->left;
    }
    return node;
}

void leftRotate(PtrRedBlackTree t, PtrRedBlackNode pivot){
    PtrRedBlackNode tempNode = pivot->right;
    
    pivot->right = tempNode->left;
    if(t->nullNode != pivot->right){
        pivot->right->parent = pivot;
    }
    if(pivot != t->root){
        if(pivot == pivot->parent->left){
            pivot->parent->left = tempNode;
        }
        else{
            pivot->parent->right = tempNode;
        }
    }
    else{
        t->root = tempNode;
    }
    tempNode->parent = pivot->parent;
    tempNode->left = pivot;
    pivot->parent = tempNode;
}

void rightRotate(PtrRedBlackTree t, PtrRedBlackNode pivot){
    PtrRedBlackNode tempNode = pivot->left;
    
    pivot->left = tempNode->right;
    if(t->nullNode != pivot->left){
        pivot->left->parent = pivot;
    }
    if(pivot != t->root){
        if(pivot == pivot->parent->left){
            pivot->parent->left = tempNode;
        }
        else{
            pivot->parent->right = tempNode;
        }
    }
    else{
        t->root = tempNode;
    }
    tempNode->parent = pivot->parent;
    tempNode->right = pivot;
    pivot->parent = tempNode;
}

void insertFixUp(PtrRedBlackTree t, PtrRedBlackNode fixNode){
    PtrRedBlackNode parent, grand, uncle;
    
    while(fixNode->parent->color != BLACK){
        parent = fixNode->parent;
        grand = parent->parent;
        if(parent == grand->left){
            uncle = grand->right;
            if(uncle->color == RED){
                parent->color = uncle->color = BLACK;
                grand->color = RED;
                fixNode = grand;
            }
            else{
                if(fixNode == parent->right){
                    leftRotate(t, parent);
                    fixNode = parent;
                    parent = fixNode->parent;
                }
                grand->color = RED;
                parent->color = BLACK;
                rightRotate(t, grand);
            }
        }
        else{
            uncle = grand->left;
            if(uncle->color == RED){
                parent->color = uncle->color = BLACK;
                grand->color = RED;
                fixNode = grand;
            }
            else{
                if(fixNode == parent->left){
                    rightRotate(t, parent);
                    fixNode = parent;
                    parent = fixNode->parent;
                }
                grand->color = RED;
                parent->color = BLACK;
                leftRotate(t, grand);
            }
        }
    }
    t->root->color = BLACK;
}

void insert(PtrRedBlackTree t, int val){
    PtrRedBlackNode newNode = allocateNode(t, val);
    PtrRedBlackNode tempNode = t->root;
    
    if(t->root == t->nullNode){
        t->root = newNode;
    }
    while(tempNode != t->nullNode){
        if(tempNode->key > val){
            if(tempNode->left == t->nullNode){
                tempNode->left = newNode;
                newNode->parent = tempNode;
                break;
            }
            else{
                tempNode = tempNode->left;
            }
        }
        else if(tempNode->key < val){
            if(tempNode->right == t->nullNode){
                tempNode->right = newNode;
                newNode->parent = tempNode;
                break;
            }
            else{
                tempNode = tempNode->right;
            }
        }
        else{
            return;
        }
    }
    insertFixUp(t, newNode);
}

void deleteFixUp(PtrRedBlackTree t, PtrRedBlackNode fixNode){
    PtrRedBlackNode parent, sibling;
    
    while(t->root != fixNode && fixNode->color != RED){
        parent = fixNode->parent;
        if(fixNode == parent->left){
            sibling = parent->right;
            if(sibling->color == RED){
                parent->color = RED;
                sibling->color = BLACK;
                leftRotate(t, parent);
                sibling = parent->right;
            }
            if(sibling->left->color == BLACK && sibling->right->color == BLACK){
                sibling->color = RED;
                fixNode = parent;
            }
            else{
                if(sibling->right->color == BLACK){
                    rightRotate(t, sibling);
                    sibling = parent->right;
                }
                sibling->right->color = BLACK;
                sibling->color = parent->color;
                parent->color = BLACK;
                leftRotate(t, parent);
                fixNode = t->root;
            }
        }
        else{
            sibling = parent->left;
            if(sibling->color == RED){
                parent->color = RED;
                sibling->color = BLACK;
                rightRotate(t, parent);
                sibling = parent->left;
            }
            if(sibling->left->color == BLACK && sibling->right->color == BLACK){
                sibling->color = RED;
                fixNode = parent;
            }
            else{
                if(sibling->left->color == BLACK){
                    leftRotate(t, sibling);
                    sibling = parent->left;
                }
                sibling->left->color = BLACK;
                sibling->color = parent->color;
                parent->color = BLACK;
                rightRotate(t, parent);
                fixNode = t->root;
            }
        }
    }
    fixNode->color = BLACK;
}

void deleteNode(PtrRedBlackTree t, int val){
    PtrRedBlackNode delNode = find(t, val);
    PtrRedBlackNode fixNode, tempNode;
    
    if(delNode == t->nullNode){
        return;
    }
    if(delNode->left == t->nullNode){
        fixNode = delNode->right;
    }
    else if(delNode->right == t->nullNode){
        fixNode = delNode->left;
    }
    else{
        tempNode = findMin(t, delNode->right);
        delNode->key = tempNode->key;
        delNode = tempNode;
        fixNode = delNode->right;
    }
    
    fixNode->parent = delNode->parent;
    if(t->root != delNode){
        if(delNode == delNode->parent->left){
            delNode->parent->left = fixNode;
        }
        else{
            delNode->parent->right = fixNode;
        }
    }
    else{
        t->root = fixNode;
    }
    
    if(delNode->color == BLACK){
        deleteFixUp(t, fixNode);
    }
}

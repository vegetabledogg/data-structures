#include <stdio.h>
#include <stdlib.h>

#define ElementType int

typedef struct SplayNode *PtrSplayNode;
struct SplayNode{
    ElementType key;
    PtrSplayNode left;
    PtrSplayNode right;
    PtrSplayNode parent;
};

typedef struct SplayTree *PtrSplayTree;
struct SplayTree{
    PtrSplayNode root;
    PtrSplayNode nullNode;
};

PtrSplayNode allocateNode(PtrSplayTree t, ElementType val);
void zig(PtrSplayTree t, PtrSplayNode pivot);
void zag(PtrSplayTree t, PtrSplayNode pivot);
void splay(PtrSplayTree t, PtrSplayNode node);
void find(PtrSplayTree t, ElementType val);
void insert(PtrSplayTree t, ElementType val);
PtrSplayNode findMax(PtrSplayTree t, PtrSplayNode node);
void join(PtrSplayTree t, PtrSplayNode lhs, PtrSplayNode rhs);
void delete(PtrSplayTree t, ElementType val);
void printTree(PtrSplayTree t);
void inOrder(PtrSplayTree t, PtrSplayNode node);
void preOrder(PtrSplayTree t, PtrSplayNode node);

int main(){
    PtrSplayTree t = (PtrSplayTree)malloc(sizeof(struct SplayTree));
    int val;
    
    t->nullNode = (PtrSplayNode)malloc(sizeof(struct SplayNode));
    t->nullNode->parent = t->nullNode->left = t->nullNode->right = NULL;
    t->nullNode->key = -1;
    t->root = t->nullNode;
    printf("Enter keys to be inserted and end with -1\n");
    while(1){
        scanf("%d", &val);
        if(-1 == val){
            break;
        }
        insert(t, val);
    }
    printTree(t);
    //Find(T, 2);
    //printf("%d ", T->root->key);
    
    printf("Enter keys to be deleted and end with -1\n");
    while(1){
        scanf("%d", &val);
        if(-1 == val){
            break;
        }
        delete(t, val);
    }
    printTree(t);
    
    return 0;
}

PtrSplayNode allocateNode(PtrSplayTree t, ElementType val){
    PtrSplayNode newNode = (PtrSplayNode)malloc(sizeof(struct SplayNode));
    
    newNode->key = val;
    newNode->left = newNode->right = t->nullNode;
    
    return newNode;
}

void zig(PtrSplayTree t, PtrSplayNode pivot){
    PtrSplayNode tempNode = pivot->left;
    
    pivot->left = tempNode->right;
    if(t->nullNode != pivot->left){
        pivot->left->parent = pivot;
    }
    tempNode->parent = pivot->parent;
    tempNode->right = pivot;
    pivot->parent = tempNode;
    if(t->root == pivot){
        t->root = tempNode;
    }
    else{
        if(pivot == tempNode->parent->left){
            tempNode->parent->left = tempNode;
        }
        else{
            tempNode->parent->right = tempNode;
        }
    }
}

void zag(PtrSplayTree t, PtrSplayNode pivot){
    PtrSplayNode tempNode = pivot->right;
    
    pivot->right = tempNode->left;
    if(t->nullNode != pivot->right){
        pivot->right->parent = pivot;
    }
    tempNode->parent = pivot->parent;
    tempNode->left = pivot;
    pivot->parent = tempNode;
    if(t->root == pivot){
        t->root = tempNode;
    }
    else{
        if(pivot == tempNode->parent->left){
            tempNode->parent->left = tempNode;
        }
        else{
            tempNode->parent->right = tempNode;
        }
    }
}

void splay(PtrSplayTree t, PtrSplayNode node){
    while(t->root != node){
        if(node->parent != t->root){
            if(node == node->parent->left && node->parent == node->parent->parent->left){
                zig(t, node->parent->parent);
                zig(t, node->parent);
            }
            else if(node == node->parent->right && node->parent == node->parent->parent->left){
                zag(t, node->parent);
                zig(t, node->parent);
            }
            else if(node == node->parent->left && node->parent == node->parent->parent->right){
                zig(t, node->parent);
                zag(t, node->parent);
            }
            else if(node == node->parent->right && node->parent == node->parent->parent->right){
                zag(t, node->parent->parent);
                zag(t, node->parent);
            }
        }
        else{
            if(node == node->parent->left){
                zig(t, node->parent);
            }
            else{
                zag(t, node->parent);
            }
        }
    }
}

void find(PtrSplayTree t, ElementType val){//要注意是否能处理树为空的情况和搜索值不在树中的情况
    PtrSplayNode tempNode = t->root;
    
    while(tempNode != t->nullNode){
        if(tempNode->key > val){
            tempNode = tempNode->left;
        }
        else if(tempNode->key < val){
            tempNode = tempNode->right;
        }
        else{
            splay(t, tempNode);
            return;//这里要注意跳出循环，否则因为经过splay操作，循环将永远不能通过while的判断条件结束
        }
    }
}

void insert(PtrSplayTree t, ElementType val){
    PtrSplayNode tempNode = t->root;
    PtrSplayNode newNode = allocateNode(t, val);
    
    if(t->nullNode == tempNode){
        t->root = newNode;
        newNode->parent = t->nullNode;
        return;
    }
    while(1){
        if(tempNode->key > val){
            if(tempNode->left != t->nullNode){
                tempNode = tempNode->left;
            }
            else{
                tempNode->left = newNode;
                newNode->parent = tempNode;
                break;
            }
        }
        else if(tempNode->key < val){
            if(tempNode->right != t->nullNode){
                tempNode = tempNode->right;
            }
            else{
                tempNode->right = newNode;
                newNode->parent = tempNode;
                break;
            }
        }
        else{
            return;
        }
    }
    splay(t, newNode);
}

PtrSplayNode findMax(PtrSplayTree t, PtrSplayNode node){
    if(t->nullNode == node){
        return node;
    }
    while(node->right != t->nullNode){
        node = node->right;
    }
    
    return node;
}

void join(PtrSplayTree t, PtrSplayNode lhs, PtrSplayNode rhs){
    PtrSplayNode tempNode = findMax(t, lhs);
    
    if(t->nullNode == tempNode){//注意考虑lhs为空的情况
        t->root = rhs;
        if(t->nullNode != rhs){//不要忘记parent指针
            rhs->parent = t->nullNode;
        }
    }
    else{
        /*
         * 因为splay函数是将待调整节点移动至根节点位置，
         * 而这里实际想要实现的是将待调整节点移动至根节点的左子节点位置，
         * 因此这里必须先将根节点改为原根节点的左子节点才能调用splay函数实现想要达到的效果，
         * 在变化根节点时要注意，除了变化root指针外，
         * 根节点还应该满足的是其parent指针需要指向nullNode
         */
        t->root = lhs;
        lhs->parent = t->nullNode;
        splay(t, tempNode);
        tempNode->right = rhs;
        if(t->nullNode != rhs){
            rhs->parent = tempNode;
        }
    }
}

void delete(PtrSplayTree t, ElementType val){
    if(t->nullNode == t->root){//除了树中没有该值这样的情况外，还应该考虑空树的情况
                                //当然这里也可以理解为指针解引用之前必须保证不为空指针
                                //虽然这里root节点指向nullNode，访问不会失败，
                                //并且只要待删除值不为-1，也能做出一定处理，但是为了保险起见需要这样处理
        printf("The tree is empty now\n");
        return;
    }
    find(t, val);
    if(t->root->key == val){
        join(t, t->root->left, t->root->right);
    }
    else{
        printf("%d not find\n", val);
    }
}

void printTree(PtrSplayTree t){
    printf("Preorder:\n");
    preOrder(t, t->root);
    putchar('\n');
    printf("Inorder:\n");
    inOrder(t, t->root);
    putchar('\n');
}

void inOrder(PtrSplayTree t, PtrSplayNode node){
    if(t->nullNode == node){
        return;
    }
    inOrder(t, node->left);
    printf("%d ", node->key);
    inOrder(t, node->right);
}

void preOrder(PtrSplayTree t, PtrSplayNode node){
    if(t->nullNode == node){
        return;
    }
    printf("%d ", node->key);
    preOrder(t, node->left);
    preOrder(t, node->right);
}

#include "redblacktree.h"

RedBlackTree::RedBlackTree()
    :root(nullptr), nullNode(nullptr){
    nullNode = new RedBlackNode(-1, nullptr, nullptr, nullptr, BLACK);
    root = nullNode;
}

void RedBlackTree::insert(int val){
    if(nullNode == root){
        root = new RedBlackNode(val, nullNode, nullNode, nullNode, BLACK);
        return;
    }

    RedBlackNode *tempNode = root;
    while(nullNode != tempNode){
        if(tempNode->key > val){
            if(tempNode->left == nullNode){
                tempNode->left = new RedBlackNode(val, nullNode, nullNode, tempNode, RED);
                insertFixUp(tempNode->left);
                return;
            }
            else{
                tempNode = tempNode->left;
            }
        }
        else if(tempNode->key < val){
            if(tempNode->right == nullNode){
                tempNode->right = new RedBlackNode(val, nullNode, nullNode, tempNode, RED);
                insertFixUp(tempNode->right);
                return;
            }
            else{
                tempNode = tempNode->right;
            }
        }
        else{
            return;
        }
    }
}

void RedBlackTree::remove(int val){
    if(nullNode == root){
        return;
    }

    RedBlackNode *tempNode = root, *successor, *fixNode;
    while(nullNode != tempNode){
        if(tempNode->key > val){
            tempNode = tempNode->left;
        }
        else if(tempNode->key < val){
            tempNode = tempNode->right;
        }
        else{
            if(nullNode != tempNode->left && nullNode != tempNode->right){
                successor = findMin(tempNode->right);
                tempNode->key = successor->key;
                tempNode = successor;
            }
            if(nullNode == tempNode->left && nullNode == tempNode->right){
                fixNode = nullNode;
            }
            else if(nullNode == tempNode->left){
                fixNode = tempNode->right;
            }
            else{
                fixNode = tempNode->left;
            }
            break;
        }
    }
    if(tempNode != nullNode){
        transplant(tempNode, fixNode);
        if(BLACK == tempNode->color){
            removeFixUp(fixNode);
        }
        delete tempNode;
    }
}

void RedBlackTree::printTree() const{
    preTraversal(root);
    cout << endl;
    inTraversal(root);
    cout << endl;
}

RedBlackTree::RedBlackNode* RedBlackTree::findMax(RedBlackNode *t){
    if(nullNode == t){
        return nullNode;
    }
    while(nullNode != t->right){
        t = t->right;
    }
    return t;
}

RedBlackTree::RedBlackNode* RedBlackTree::findMin(RedBlackNode *t){
    if(nullNode == t){
        return nullNode;
    }
    while(nullNode != t->left){
        t = t->left;
    }
    return t;
}

void RedBlackTree::leftRotate(RedBlackNode *t){
    RedBlackNode *tempNode = t->right;

    t->right = tempNode->left;
    if(nullNode != t->right){
        t->right->parent = t;
    }
    if(nullNode != t->parent){
        if(t == t->parent->left){
            t->parent->left = tempNode;
        }
        else{
            t->parent->right = tempNode;
        }
    }
    else{
        root = tempNode;
    }
    tempNode->parent = t->parent;
    tempNode->left = t;
    t->parent = tempNode;
}

void RedBlackTree::rightRotate(RedBlackNode *t){
    RedBlackNode *tempNode = t->left;

    t->left = tempNode->right;
    if(nullNode != t->left){
        t->left->parent = t;
    }
    if(nullNode != t->parent){
        if(t == t->parent->left){
            t->parent->left = tempNode;
        }
        else{
            t->parent->right = tempNode;
        }
    }
    else{
        root = tempNode;
    }
    tempNode->parent = t->parent;
    tempNode->right = t;
    t->parent = tempNode;
}

void RedBlackTree::transplant(RedBlackNode *t, RedBlackNode *fixNode){
    fixNode->parent = t->parent;
    if(t != root){
        if(t == t->parent->left){
            t->parent->left = fixNode;
        }
        else{
            t->parent->right = fixNode;
        }
    }
    else{
        root = fixNode;
    }
}

void RedBlackTree::insertFixUp(RedBlackNode *t){
    RedBlackNode *parent, *grand, *uncle;

    while(BLACK != t->parent->color){
        parent = t->parent;
        grand = parent->parent;
        if(parent == grand->left){
            uncle = grand->right;//由于有nullNode作为sentinel，所以uncle节点一定不为nullptr
            //Case1
            if(RED == uncle->color){
                parent->color = uncle->color = BLACK;
                grand->color = RED;
                t = grand;
            }
            else{
                if(t == parent->right){
                    leftRotate(parent);
                    t = parent;
                    parent = t->parent;
                }
                parent->color = BLACK;
                grand->color = RED;
                rightRotate(grand);
            }
        }
        else{
            uncle = grand->left;
            if(RED == uncle->color){
                parent->color = uncle->color = BLACK;
                grand->color = RED;
                t = grand;
            }
            else{
                if(t == parent->left){
                    rightRotate(parent);
                    t = parent;
                    parent = t->parent;
                }
                parent->color = BLACK;
                grand->color = RED;
                leftRotate(grand);
            }
        }
    }
    root->color = BLACK;//因为从Case1跳出循环时有可能是t为root但其color仍然为RED，
                        //这时为了满足红黑树的性质就必须将root重新赋值为BLACK，
                        //最终结果相当于红黑树中所有到nullNode的路径上的BLACK节点数都加一
}

void RedBlackTree::removeFixUp(RedBlackNode *t){
    RedBlackNode *parent, *sibling;

    while(root != t && BLACK == t->color){
        parent = t->parent;
        if(t == parent->left){
            //sibling节点一定存在，因为如果不存在则为nullNode，
            //那么未删除前从待删除节点到nullNode的路径上的BLACK节点数（至少为2）
            //就大于从sibling节点到nullNode的路径上的BLACK节点数（只有1，即sibling节点自己）
            //这样是违背红黑树性质的
            sibling = parent->right;
            if(RED == sibling->color){
                parent->color = RED;
                sibling->color = BLACK;
                leftRotate(parent);
                sibling = parent->right;
            }
            //Case2
            if(BLACK == sibling->left->color && BLACK == sibling->right->color){//有sentinel存在，所以这样的访问一定可行
                sibling->color = RED;
                t = parent;
            }
            else{
                //sibling的左子节点为RED则说明其一定不为nullNode，则可以进行旋转操作
                if(BLACK == sibling->right->color){
                    rightRotate(sibling);
                    sibling = sibling->parent;
                }
                //不论一开始sibling的右子节点是否为nullNode，此时其一定不为nullNode，
                //因为就算一开始为nullNode经过对sibling的旋转sibling的右子节点一定不为nullNode，
                //如果一开始sibling的右子节点是RED就将其赋值为BLACK，
                //如果一开始sibling的右子节点不存在，那么经过对sibling的旋转以后color已经是BLACK，再次赋值也没有副作用
                //并且Case4的相应处理本来就需要使得sibling的右子节点的color为BLACK
                sibling->right->color = BLACK;
                sibling->color = parent->color;
                parent->color = BLACK;
                leftRotate(parent);
                t = root;
            }
        }
        else{
            sibling = parent->left;
            if(RED == sibling->color){
                parent->color = RED;
                sibling->color = BLACK;
                rightRotate(parent);
                sibling = parent->left;
            }
            if(BLACK == sibling->left->color && BLACK == sibling->right->color){
                sibling->color = RED;
                t = parent;
            }
            else{
                if(BLACK == sibling->left->color){
                    leftRotate(sibling);
                    sibling = sibling->parent;
                }
                sibling->left->color = BLACK;
                sibling->color = parent->color;
                parent->color = BLACK;
                rightRotate(parent);
                t = root;
            }
        }
    }
    t->color = BLACK;//这里必须是t而不是root，
                    //因为循环可能是因为t的color为RED时结束的，并且此时t并不为root
                    //最简单的例子就是当删除节点后接上去的节点为RED，这样不进入循环直接就跳到了这一步
                    //或者是Case2，Case2中parent的color任意，当为RED时则跳出循环
                    //而对应与这样的情况，将t的color赋值为BLACK即可满足红黑树的性质
}

void RedBlackTree::preTraversal(RedBlackNode *t) const{
    if(nullNode == t){
        return;
    }

    cout << t->key << "-" << t->color << " ";
    preTraversal(t->left);
    preTraversal(t->right);
}

void RedBlackTree::inTraversal(RedBlackNode *t) const{
    if(nullNode == t){
        return;
    }

    inTraversal(t->left);
    cout << t->key << "-" << t->color << " ";
    inTraversal(t->right);
}

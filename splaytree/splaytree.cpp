#include "splaytree.h"

SplayTree::SplayTree()
    :root(nullptr){}

SplayTree::~SplayTree(){
    makeEmpty(root);
}

int SplayTree::findMax() const{
    SplayNode *tempNode = findMax(root);
    if(nullptr == tempNode){
        return -1;
    }
    return tempNode->key;
}

int SplayTree::findMin() const{
    SplayNode *tempNode = findMin(root);
    if(nullptr == tempNode){
        return -1;
    }
    return tempNode->key;
}

void SplayTree::insert(int val){
    if(nullptr == root){
        root = new SplayNode(val, nullptr, nullptr, nullptr);
        return;
    }

    SplayNode *tempNode = root;
    while(nullptr != tempNode){
        if(tempNode->key > val){
            if(nullptr == tempNode->left){
                tempNode->left = new SplayNode(val, nullptr, nullptr, tempNode);
                splay(tempNode->left);
                return;
            }
            else{
                tempNode = tempNode->left;
            }
        }
        else if(tempNode->key < val){
            if(nullptr == tempNode->right){
                tempNode->right = new SplayNode(val, nullptr, nullptr, tempNode);
                splay(tempNode->right);
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

void SplayTree::remove(int val){
    SplayNode *tempNode = find(val, root);

    if(nullptr == tempNode){
        return;
    }
    splay(tempNode);
    join(root->left, root->right);
}

void SplayTree::printTree() const{
    printTree(root);
    cout << endl;
}

void SplayTree::zig(SplayNode *t){
    SplayNode *tempNode = t->left;
    t->left = tempNode->right;
    if(nullptr != t->left){
        t->left->parent = t;
    }
    tempNode->right = t;
    if(nullptr != t->parent){
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
    t->parent = tempNode;
}

void SplayTree::zag(SplayNode *t){
    SplayNode *tempNode = t->right;
    t->right = tempNode->left;
    if(nullptr != t->right){
        t->right->parent = t;
    }
    tempNode->left = t;
    if(nullptr != t->parent){
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
    t->parent = tempNode;
}

void SplayTree::splay(SplayNode *t){
    while(root != t){
        if(t == t->parent->left){
            if(nullptr != t->parent->parent){
                if(t->parent == t->parent->parent->left){
                    zig(t->parent->parent);
                    zig(t->parent);
                }
                else{
                    zig(t->parent);
                    zag(t->parent);
                }
            }
            else{
                zig(t->parent);
            }
        }
        else{
            if(nullptr != t->parent->parent){
                if(t->parent == t->parent->parent->right){
                    zag(t->parent->parent);
                    zag(t->parent);
                }
                else{
                    zag(t->parent);
                    zig(t->parent);
                }
            }
            else{
                zag(t->parent);
            }
        }
    }
}

SplayTree::SplayNode* SplayTree::findMax(SplayNode *t) const{
    if(nullptr == t){
        return nullptr;
    }
    while(nullptr != t->right){
        t = t->right;
    }
    return t;
}

SplayTree::SplayNode* SplayTree::findMin(SplayNode *t) const{
    if(nullptr == t){
        return nullptr;
    }
    while(nullptr != t->left){
        t = t->left;
    }
    return t;
}

SplayTree::SplayNode* SplayTree::find(int val, SplayNode *t) const{
    while(nullptr != t){
        if(t->key > val){
            t = t->left;
        }
        else if(t->key < val){
            t = t->right;
        }
        else{
            return t;
        }
    }
    return t;
}

void SplayTree::join(SplayNode *lhs, SplayNode *rhs){
    SplayNode *tempNode = findMax(lhs);
    if(nullptr == tempNode){
        root = rhs;
        if(nullptr != rhs){
            rhs->parent = nullptr;
        }
    }
    else{
        root = lhs;
        lhs->parent = nullptr;
        splay(tempNode);
        tempNode->right = rhs;
        if(nullptr != rhs){
            rhs->parent = tempNode;
        }
    }
}

void SplayTree::printTree(SplayNode *t) const{
    if(nullptr == t){
        return;
    }
    printTree(t->left);
    cout << t->key << " ";
    printTree(t->right);
}

void SplayTree::makeEmpty(SplayNode *&t){
    if(nullptr != t){
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}

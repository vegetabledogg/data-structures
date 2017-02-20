## Red Black Tree

[Pic001]:http://i1.piimg.com/567571/c691f56bb4c9adfc.jpg
[Pic002]:http://p1.bpimg.com/567571/21fa23c594a7a6cf.jpg
[Pic003]:http://i1.piimg.com/567571/94b55961968cc7e7.jpg
[Pic004]:http://p1.bqimg.com/567571/46d624cac0becc72.png

### Index

-----

+ [Basic](#basic)
+ [Rotate](#rotate)
+ [Insert](#insert)
+ [Delete](#delete)

### Basic

-----

红黑树的节点声明，其中Parent指针是指向某一节点的父节点的指针：

```C
typedef struct RedBlackNode *PtrRedBlackNode;
struct RedBlackNode{
    ElementType key;
    PtrRedBlackNode left;
    PtrRedBlackNode right;
    PtrRedBlackNode parent;
    int color;
};
```

红黑树的总体声明，该声明中包含了指向红黑树根节点的指针和指向用作sentinel的dummy node的指针：

```C
typedef struct RedBlackTree *PtrRedBlackTree;
struct RedBlackTree{
    PtrRedBlackNode root;
    PtrRedBlackNode nullNode;
};
```

一些其他的相关函数：

```C
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
```

当然还有一个删除单支节点时需要用到的函数，这个函数将被删除节点的子树接上去，需要注意的是不要遗漏对于Parent指针的操作：

```C
void transplant(PtrRedBlackTree t, PtrRedBlackNode u, PtrRedBlackNode v){
    if(t->nullNode == u->parent){
        t->root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}
```

正是由于该函数的存在，删除黑色叶节点的情况才能归入删除黑色单支节点中。因为在下文可以看到删除后向上调整的过程需要访问待调整节点的父节点，而如果删除的是黑色叶节点，起初待调整节点就变为了sentinel节点，而sentinel节点的```Parent```指针一开始是无确定指向的，通过在删除过程中调用```RBTransPlant```函数，可以使得sentinel节点的```Parent```指针指向待删除节点的父节点，这样就可以将删除黑色叶节点看作删除黑色单支节点的情况来处理了。

这个函数还需要注意，如果被删除节点为根节点，就需要改变```t->root```的值，而不是去修改原来指向待删除节点的left或right指针，因为原来就没有节点指向待删除节点。

-----

### Rotate

-----

红黑树的旋转操作和AVL树的旋转操作差不多，但是还是有几个需要特别注意的地方。

首先，应当注意红黑树的每个节点都有parent指针，在旋转操作时不能遗漏对于parent指针的操作。

第二，对于某一节点中parent指针的操作需要访问该节点，这时就应当注意该节点是否为NULL节点。例如下图中的C节点就可能为NULL节点，如果不是NULL节点，在旋转时要将C的parent指针指向B。当然在处理NULL节点时，我们可以利用一个dummy node来作为一个sentinel，所有的叶节点的left和right指针都指向这个sentinel，而根节点的parent指针也指向sentinel，sentinel的color为Black，其余成员值为任意。

![Insert.Case.2][Pic002]

第三，在旋转时应该注意根节点。当旋转的pivot节点就是根节点时，应当注意更改```struct RedBlackTree```结构中的root指针，将其指向新的根节点。当旋转的pivot节点不是根节点时，应当注意更改pivot的父节点的left或right指针，这里就需要加以分类讨论（到底新的根节点，如上图中的D节点，是其父节点的左子树还是右子树），可以利用```pivot->Parent```来访问pivot的父节点。

###### Source Code

```C
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
```

-----

### Insert

-----

红黑树的插入操作和BST也差不多，同样在插入以后需要像AVL树那样向上调整，但是红黑树因为每个节点都存在parent指针，所以向上调整可以通过迭代来实现，而不需要像AVL树那样要用递归回溯。红黑树向上调整的过程实际上就是不断将新插入的红节点向上移动，直至它的父节点为黑为止，这样就存在三种情况（之所以不存在其他情况，完全是由于红黑树的性质决定的）。并且红黑树的根节点和根节点的父节点的color一定是Black，所以这个向上调整的过程就一定会停止，也就是最终一定能跳出循环，在跳出循环之后需要将根节点的color赋值为Black。

**补充**

因为红黑树插入后向上调整的操作总是能够保证调整所涉及的节点与其sibling节点到叶节点的路径上的BLACK节点数相同，而调整操作实际要解决的问题是父子节点均为RED的问题，并且向上调整的过程中fixNode的color一定为RED，所以只要fixNode的父节点的color为BLACK，调整就完成了，树就能满足红黑树的相应性质，因此循环结束的条件也就是fixNode的父节点的color为BLACK。

以下三种情况均针对待调整节点在其祖父节点的左子树中时进行分析，若在右子树中时，做对称操作即可。

###### Case One

这种情况中,待调整节点是C节点，C节点的父节点B和父节点B的sibling节点E的color均为Red（需要注意的是这里C、D、E的左右子树可能为空，可能不为空，且A节点也可能不是根节点）。遇到这种情况时，将C节点的父节点B和父节点B的sibling节点E的color赋值为Black，并将C节点的祖父节点A赋值为Red，同时将待调整节点变为节点A。因为起初父节点的color为Red，所以根据性质，C的祖父节点A的color一定为Black，这样同时调整B和E为Black，可以使沿B和E至叶节点的路径上的Black节点数相同，且消除了B、C均为Red的情况，但是这样一来沿A至叶节点的路径上的Black节点数就增加了一个，因此将A赋值为Red，使得沿A至叶节点的路径上的Black节点数保持和调整前的数量一致，然而我们无法排除A的父节点的color也是Red的情况，所以将待调整节点变为节点A，在下一次循环中继续调整。

![Insert.Case.1][Pic001]

###### Case Two

这种情况中，待调整节点是D节点，D节点的父节点B的color是Red，但是父节点B的sibling节点E的color是Black，且D节点是其父节点B的右子节点。此时仅需要以B节点为pivot做左旋即可，并将待调整节点变为B。在具体实现时还需要注意将记录父节点的变量```parent```变为D节点，并进入Case Three。之所以要这样操作，是因为这里的操作仅仅针对两个Red节点，而对于Black节点的操作（例如C节点），起初沿B的左子节点、D的左（C节点）右子节点至叶节点的路径的Black节点数都是相同的，所以旋转操作中移动以C为根节点的子树后，沿B的子节点和D的子节点至叶节点的路径的Black节点数依然是相同的且保持不变。

![Insert.Case.2][Pic002]

###### Case Three

这种情况中，待调整节点是C节点，C节点的父节点B的color是Red，但是父节点B的sibling节点E的color是Black，且C节点是其父节点B的左子节点。此时仅需要以C节点的祖父节点A作为pivot做右旋即可，并将原先的父节点B的color调整为Black，将原先的祖父节点A的color调整为Red。之所以要这样操作，是因为起初沿C节点的左右子节点、沿D节点和E节点至叶节点的路径的Black节点数是相同的，所以在调整过后沿它们至叶节点的路径上的Black节点数依然是相同的且保持不变，而这样操作却可以通过交换颜色将一个Red节点移动到祖先节点的右子树中，消除了两个Red节点相连的情况，当然旋转后新的子树的根节点B要赋值为Black，以保持从子树的根节点（原先是A，现在是B）的路径的Black节点数保持和原来一样。这里要是pivot是整棵红黑树的根节点，则需更新root节点的值。

![Insert.Case.3][Pic003]

###### Source Code

```C
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
            return;//这里必须立即返回，否则程序有可能会继续执行导致死循环，
            		//而如果仅仅是跳出循环的话又会使得insertFixUp无效执行
        }
    }
    insertFixUp(t, newNode);
}
```

-----

### Delete

-----

红黑树的删除和BST的删除也是类似的，区别在于红黑树删除后需要保持性质。这就同样需要在删除过后进行调整。***而红黑树的删除主要是对于单支黑节点（即一个黑节点只有左子树或只有右子树）的操作。***

1. 因为删除红色叶节点对于红黑树的性质没有影响，删除黑色叶节点因为存在着sentinel节点，所以可以归入删除黑色internal节点的情况。

2. 而对于删除internal节点，则分为左右子树均非空的节点，单支黑节点，单支红节点三种情况：

+ 如果该internal节点左右子树均非空，则像BST那样在右子树中找中缀后继节点，用后继节点的值来替换待删除的internal节点的值，internal节点的其余成员值保持不变，就相当于删除了该节点，同时继续对后继节点进行删除操作，实际上就可以归入删除单支节点的情况；
+ 如果该internal节点为单支红节点，则不论其子节点是红是黑，均不符合红黑树的性质，所以实际上并不存在此种情况；
+ 如果该internal节点为单支黑节点，那么像BST那样将其子树接上，就相当于是删除了该节点。而这样一来，因为删除了一个黑节点，对于红黑树的第五个性质造成了破坏，这里就需要对红黑树进行调整。

因此实际上在具体实现中我们仅在被删除节点为黑节点时进行调整，所以在具体实现的```void deleteNode(PtrRedBlackTree t, int val)```中，需要用一个临时变量```delNode```来记录待删除节点，并在删除执行完之后判断待删除节点的颜色，以便确定是否需要调用```void deleteFixUp(PtrRedBlackTree t, PtrRedBlackNode fixNode)```函数进行调整：

```C
    if(delNode->color == BLACK){
        deleteFixUp(t, fixNode);
    }
```

而以上所说的调整则是需要根据被删除的单支黑节点的子节点的color来判断的。如果其子节点是黑色的，则有四种不同的情况，在```deleteFixUp```函数的具体实现中需要进入一个while循环来处理，如果其子节点是红色的，则无需进入循环，直接执行循环后的```fixNode->color = BLACK;```，就是将其子节点的color变为黑色，相当于增加了一个黑节点来消除删除一个黑节点对红黑树性质的影响。

对于四种不同情况，《算法导论》上介绍的很明白了，下面主要说明Case2、4的情况，并通过注释来介绍Case1、3和具体实现：

当然需要特别注意的是，所谓的待调整节点其实就是沿待调整节点比沿待调整节点的sibling节点至叶节点的Black节点数少1，所以在调整过程中我们只在待调整节点为黑且不为根节点时做调整，因为待调整节点为红时仅需将其置为黑即能保持红黑树性质，而待调整节点为根节点时相当于这一次删除最终使得整棵红黑树中根节点至叶节点的路径的Black节点数均减少1，但仍然符合红黑树性质。

![Delete.Cases][Pic004]

###### Case Two

此情况下，不论父节点B是何种颜色，均将待调整节点A的sibling节点D颜色置为Red，这样可以使上图情况2中沿A节点和D节点至叶节点的路径具有相同的Black节点数，但是却使得图中沿B节点至叶节点的路径的Black节点数减少1，这样相当于B节点等价于一个待调整节点，因为***_待调整节点的一个特点就是沿待调整节点比沿待调整节点的sibling节点至叶节点的Black节点数少1_***，因此将待调整节点变为B即可，并继续循环，若B为红则跳出循环后将其颜色置为黑，若B为黑则继续循环判断是Case1、2、3、4中的何种情况。

###### Case Four

这里注意是将图中D节点的颜色变为B节点原先的颜色，可黑可红，以保持旋转后子树的根节点的颜色不会变化，以免对红黑树的性质造成进一步影响。同时，**(1)** _旋转之前_，沿B_至_A再到叶节点的路径比沿B的sibling节点至叶节点的路径的Black节点数少1，**(2)** 沿A节点和沿C、E节点至叶节点的Black节点数相等，这样一来，_旋转之后_，沿新的子树根节点D至叶节点的所有路径均比沿D的sibling节点至叶节点的路径的Black节点数少1，而以D为根的子树自身却已经满足了红黑树的性质，所以只需要将B节点和E节点置为Black即可使整棵树满足红黑树性质，最后将待调整节点赋值为```t->root```即可在下一次跳出循环。

###### Source Code

```C
//执行删除操作，与算法导论上给的伪代码不一样
void deleteNode(PtrRedBlackTree t, int val){
    PtrRedBlackNode delNode = find(t, val);
    PtrRedBlackNode fixNode, tempNode;
    
    //注意delNode不存在可能有两种情况：
    //树为空和树中不存在待删除的值
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

void deleteFixUp(PtrRedBlackTree t, PtrRedBlackNode fixNode){
    PtrRedBlackNode parent, sibling;
    
    while(t->root != fixNode && fixNode->color != RED){
        parent = fixNode->parent;
        if(fixNode == parent->left){
            sibling = parent->right;
            /*
            对应于Case1，而Case1调整后即为Case2、3、4中的任意一种，
            所以如果满足Case1的情况执行if语句调整后，
            程序流继续执行以判断为Case2、3、4中的哪一种情况
            注意此时待调整节点的sibling节点已经发生变化，
			  为了继续处理之后的情况需要更新SLNode的值
             */
            if(sibling->color == RED){
                parent->color = RED;
                sibling->color = BLACK;
                leftRotate(t, parent);
                sibling = parent->right;
            }
            /*
            以下是Case2、3、4，判断标准是待调整节点的sibling的子节点的color，
            第一个if对应于Case2
             */
            if(sibling->left->color == BLACK && sibling->right->color == BLACK){
                sibling->color = RED;
                fixNode = parent;
            }
            /*
            对应于Case3、4
             */
            else{
            	   /*
            	   对应于Case3，而Case3调整后即为Case4，
                所以如果满足Case3的情况执行if语句调整后，
                程序流继续执行以处理Case4的情况，
                注意此时待调整节点的sibling节点已经发生变化，
                为了继续处理Case4需要更新SLNode的值。
                还应该注意的是只要sibling节点的右子节点为红就属于Case4，
                即Case4时sibling的左子节点不论红黑都属于Case4，
                因此Case3的判断条件应该是sibling的右子节点为Black
                 */
                if(sibling->right->color == BLACK){
                    rightRotate(t, sibling);
                    sibling = parent->right;
                }
                /*
            	   对应于Case4
                 */
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
    /*
    当待调整节点最初就是红节点，或Case2中新的待调整节点为红节点（上图中情况2的new x为红），Case4中待调整节点变为根节点时，跳出循环，执行此语句
     */
    fixNode->color = BLACK;
}
```
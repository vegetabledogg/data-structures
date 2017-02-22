## B+ Tree

### index

-----

+ [Basic](#basic)
+ [Insert](#insert)
+ [Delete](#delete)

-----

### Basic

-----

B+树和B树类似（，区别主要在于叶节点，如果在父节点的```child```数组中指向某一叶节点指针的下标为index，则该叶节点中的最大数据值与其父节点中```child[index]```的值相等，并且除最右侧的叶节点之外所有叶节点都有一个指针指向其右边的兄弟节点，因此所有非叶节点中数据值都在叶节点中有相同的值与之对应。

下面是一些声明和定义：

```C
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

int getindex(PtrElementType key, int size, ElementType val){
    int i;
    
    for(i = 0; i < size; i++){
        if(key[i] >= val){
            break;
        }
    }
    
    return i;
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
```
-----

### Insert

-----

B+树的插入只需要在B树插入的基础上处理叶节点的特殊情况。所以差异的部分主要是分裂节点的函数：

```C
void spilit(PtrBplusNode node, int childIndex){
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
```
这个函数将叶节点的分裂和非叶节点的分裂作为两种情况来处理，而实际上这个函数还是可以优化的。非叶节点的分裂和B树的一样；叶节点的分裂则是将满叶节点分裂为数据量为Minimum Degree和Minimum Degree - 1的两个节点，同时将大小为Minimum Degree的节点中的最大数据（其实就是原满叶节点的数据中值）向上插入到该叶节点的父节点中，当然还需要将叶节点中指向兄弟节点的指针进行赋值，相当于是单链表中的插入操作。与B树一样，这里也需要注意对于节点结构中```isLeaf```成员的赋值。对于满叶节点的分裂，与B树的区别就在于B树是将满叶节点分裂为数据量均为Minimum Degree - 1的两个节点，因此同样是将原满叶节点的数据中值向上插入到其父节点中，B+树在执行完分裂叶节点的操作后，该数据中值仍然保留在分裂后的某一叶节点中，而B树在执行完分裂叶节点的操作后，相当于是把该数据中值从叶节点中删除了。

完整的插入操作：

```C
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
            spilit(node, index);
            if(node->key[index] < val){//Caution
                index++;
            }
        }
        insertNonFull(node->child[index], val);
    }
}

void spilit(PtrBplusNode node, int childIndex){
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
```
**记录自己码代码过程中的一个小bug**

当然以上操作中的```BpInsertNonFull```函数，我自己在第一遍写的时候出过一个小bug，对于以上代码中的Caution注释行处的内层if语句块，我一开始没有将其放在外层的if语句块中，而是放在外层if语句块外面，紧接着外层if语句执行，但是应当注意到此函数中的```index```变量有可能等于数组的size，因此如果内层if语句块放在外面，会使得无论是否执行分裂都要通过index来访问数组（在if条件判断中），这样就可能出现越界的情况，而现在这样的处理使得只有执行分裂操作后才会通过index来访问数组，而分裂后数组size加1，也就不存在越界的问题了。

-----

### Delete

-----

B+树的删除虽然有很多情况需要处理，但是其中的一部分都与B树相同，这里只记录不同于B树的情况处理，主要也是为了维护B+树叶节点的特有性质。

1. 待删除数据```val```在该节点中，且该节点的子节点是叶节点，记该节点为```currentNode```，待删除数据```val```在该节点的```child```数组中的下标为index，并记该节点的```child```数组中下标为index的元素所指向的节点为```precursor```，下标为index + 1的元素所指向的节点为```successor```。

   + 如果```precursor```中的数据量大于Minimum Degree - 1，用```precursor```中第二大的数据值代替```currentNode```中的```val```，并在```precursor```中递归删除```val```。
   
   + 如果```successor```中的数据量大于Minimum Degree - 1，用```successor```中的最小数据值代替```currentNode```和```precursor```中的```val```值，并在```successor```中递归删除```val```。
   
   + 如果以上均不满足，则合并```precursor```和```successor```，然后在合并后的新节点中递归删除```val```。

2. 待删除数据```val```不在该节点中，且该节点的子节点是叶节点，记该节点为```currentNode```，待删除数据```val```在该节点的```child```数组中的下标为index，并记该节点的```child```数组中下标为index的元素所指向的节点为```subNode```，下标为index - 1的元素所指向的节点为```precursor```（如果存在），下标为index + 1的元素所指向的节点为```successor```（如果存在）。

   + 如果```subNode```中的数据量大于Minimum Degree - 1，则直接在```subNode```中递归删除即可。

   + 如果```subNode```中的数据量小于或等于Minimum Degree - 1：
   
      + 如果```precursor```中的数据量大于Minimum Degree - 1，则将```currentNode```中下标为index - 1的数据值插入```subNode```中，并将```subNode```中记录数据量的成员```keyNum```加1，用```precursor```中的第二大数据值填入```currentNode```中下标为index - 1的空缺，并将```precursor```中记录数据量的成员```keyNum```减1，最后在```subNode```中递归删除```val```。

      + 如果```successor```中的数据量大于Minimum Degree - 1，则将```successor```中的最小数据插入```subNode```中，并将```subNode```中记录数据量的成员```keyNum```加1，同时```currentNode```的```child```数组中下标为index的元素值也由```successor```中的最小数据代替，并将```successor```中```child```数组的部分元素向左移动一位，相应的```successor```中记录数据量的成员```keyNum```也应减1，最后在```subNode```中递归删除```val```。

      + 如果以上均不符合，则将```subNode```与```precursor```或```successor```合并（两者不一定均存在，选择存在的进行合并），然后在合并后的节点中递归删除```val```。

当然删除节点操作中的部分情况涉及移动数组的部分元素，尤其是对于内点，要注意除了```child```数组，还要移动```child```数组。

当然因为叶节点的特殊性质，合并操作也有所不同，区别就在于合并叶节点时，合并后节点的大小为Minimum Degree * 2 - 2，因为两被合并叶节点在其父节点中所夹元素同样存在于叶节点中，所以在合并中也就不需要将这个值重复插入合并节点中了。

完整的删除操作：
   
```C
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
    
    if(currentNode == t->root && 0 == currentNode->keyNum){//Caution
        t->root = leftNode;
    }
}
```
**记录自己码代码过程中的bug**

注意上面代码中的Caution注释行处。当需要合并根节点的两个子节点且合并前根节点仅有一个key时，因为合并后根节点没有key，所以需要改变```root```的值，使其指向原```root```的最左侧的子节点，这样的判断操作应该在```merge```函数中进行，因为只有在经过```merge```后使根节点没有key才能改变根节点的指向，其它情况，比如当树仅有一个根节点并且不断删除时也会造成根节点没有key，这样的情况下是不能使```root```指向```root```最左边的子节点的，因为这样会使得```root```的值为NULL，而无法在接下来继续插入key（在初始状态下```root```必须指向一个空节点，否则无法插入）。如果不在```merge```中进行，而在每次执行完```deleteFix```函数后通过判断根节点是否有key来决定是否改变根节点的指向，就可能对上述两种情况进行同样的操作而导致错误。

并且在判断待删除值是否在节点中时，需要注意的是通过```getIndex```函数得到的下标值可能超过数组的size，所以在比较得到的下标值位置的元素与待删除元素是否相同之前，需要判断```index```是否小于size，如果等于size则待删除值一定不存在。同时，```index```小于size并不代表待删除值一定存在，因为```getindex```函数返回的是大于或等于待删除值的元素中最小值的下标，因此并不一定相等。

-----
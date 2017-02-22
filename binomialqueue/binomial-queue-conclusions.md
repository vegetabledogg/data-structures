## Binomial Queue

-----

### Index

-----

- [Basic](#basic)
- [Merge](#merge)
- [Insert](#insert)
- [Delete](#delete)

-----

### Basic

-----

二项队列实际上是一个由若干满足堆序的树组成的森林，由一个数组将所有的树联系在一起，数组中的每个元素都是一个指向某棵树根节点的指针，而数组元素的下标对应所指向树的高度，每一高度只能有一棵树存在，如果因为插入或删除等等操作导致森林中有两棵树具有相同的高度，就将其中一棵树的根节点作为另一棵树根节点的儿子，由此使得树的高度增加一。

一些必要的函数：

```c
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

void print(PtrBonimialQueue queue){  
    int i;
    
    for (i = 0; i < MaxTree; i++){ 
        printf("Tree %d:\n",i); 
        preTraversal(queue->theTrees[i]);
        printf("\n");
    }  
    printf("\n");  
    return;  
}  

void preTraversal(PtrBonimialNode t){
    if (NULL == t){
        return;
    }
    
    printf("%d ", t->key);  
    preTraversal(t->firstChild);
    preTraversal(t->nextSibling);
}
```

其中```findMin(PtrBonimialNode theTrees[])```函数需要注意，在```theTrees```数组中并非所有高度的树均存在，也就是说可能存在数组中某元素指向为NULL的情况，因此需要另加判断。
而```init()```函数新建一个二项队列的结构，在这个函数中需要对新建的结构进行初始化，而初始化时要注意除了要将currentSize赋值为0外，还需要将```theTrees```数组中的所有元素赋值为NULL，如果不做这样的初始化，在之后的合并和插入操作中就会出问题。

-----

### Merge

-----

合并操作首先是两棵树的合并，这里只需要比较两棵树的根节点key值的大小，将key值较大的树的根节点作为另一棵树的根节点的子节点，当然这里需要注意树在二项队列中实际是通过firstchild-nextsibling这样的形式实现的，并且一棵树中以同一level的子节点为根节点的子树满足size从左到右递减的规律，具体如下图所示：
![pic001](http://i1.piimg.com/567571/2a1e081a574a3656.png)
因此key值较大的树一定是作为key值较小的树的根节点的最左侧的子树。具体代码如下：

```c
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
```
其次是两个二项队列的合并，因为一个二项队列中某一高度的树的数量只可能为1或0，两个二项队列合并就相当于是两个二进制数相加。如果两个二项队列中同一高度的树均存在，则对两棵树执行合并操作，并使得高度加一，因此需要赋值给数组中的下一个元素，相当于二进制加法中两个同位的1相加进位至下一位。而具体实现中，从小到大地将两个二项队列中```theTree```数组的对应下标元素所指向的树以及作为上一位进位的树进行合并（这里就体现了初始化时将数组所有元素都赋值为NULL的作用，如果不赋值为NULL，指针可能不为空指针，这样程序就会将某本来为空的指针看作指向某棵树而进行合并，造成错误）。当然和二进制加法一样，一开始时进位为0，也就是一开始时需要将```carry```变量赋值为NULL。并且除了对theTrees数组进行操作外，函数首先需要做的就是将两个二项队列的```currentSize```相加，以此来判断是否会出现overflow的情况（相当于两个二项队列合并后出现高度大于二项队列所能容纳的最大高度的树，也就是其高度大于theTrees数组下标）。通过相加后的size来判断实际上也和二进制加法的原理类似，只要两个二进制数相加的和在所能表示的最大范围内就不会overflow。具体代码如下：

```c
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
```

在第一次写这个程序时，这个函数出现了两个bug。一个是没有检查```lhs->theTree[i]```和```rhs->theTree[i]```是否存在（指向树的根节点还是NULL，即为“1”还是为“0”），而是检查了```lhs```和```rhs```是否存在，造成了错误。第二个是没有在```lhs->theTree[i]```和```carry```合并并进位后将```lhs->theTree[i]```赋值为NULL，相当于执行进位后原树还保留在原位（虽然树的实体只有一个，但是有两个指针指向其根节点，相当于这棵树被算了两次，即存在于原位，也通过进位存在于下一位），显然是不对的。在这个函数的不同情况的处理中，一要注意```carry```的值的改变，二就是要注意```lhs->theTrees[i]```的值是否需要赋值为NULL（因为函数最终返回的是lhs，也就是实际上是将rhs所指向的二项队列加到lhs上，rhs中数组元素的值是否改变可以不做太多考虑）。

-----

### Insert

-----

插入操作相当于是用待插入的值构建一个新的二项队列，再将这个新的二项队列和原二项队列合并。当然构建新的二项队列时需要注意，初始化后往二项队列添加节点，不要忘了将currentSize赋值为1。具体代码如下：

```c
void insert(PtrBonimialQueue queue, ElementType val){
    PtrBonimialQueue newQueue = init();
    newQueue->theTrees[0] = allocateNode(val);
    newQueue->currentSize = 1;
    merge(queue, newQueue);
}
```

-----

### Delete

-----

删除二项队列中的最小元素，首先需要在二项队列中找到最小元素的位置，因为每棵树的根节点均是其所在子树的最小节点，因此只需要在所有树的根节点中寻找即可，通过```findMin```函数实现。
在找到最小节点后，将最小节点的子树分离开来构建一个新的二项队列，然后将新的二项队列与原二项队列相加。这里有几点需要注意的地方，因为这里涉及指针的操作，所以在对指针解引用之前都需要考虑指针是否指向NULL。而因为在firstchild-nextsibling结构中，所有用于构建新的二项队列的子树的根节点都通过nextSibling指针相联系，在将这些子树的根节点依次赋值给新的二项队列的theTrees数组元素后，需要将这些nextSibling指针赋值为NULL以满足树的结构要求。同时不要忘记将原二项队列theTrees数组中相应位置的元素赋值为NULL，否则会出现和合并中类似的bug，即树依然存在于原位上，并没有被删除，有两个指针指向同一个树的实体，只不过这个树的实体中根节点的子树构成的链表被分裂了而已。还注意在合并之前还要调整原二项队列的currentSize，并且应当处理原二项队列为空时的情况。具体代码如下：

```c
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
    //这个for循环不必判断tempNode是否为NULL
    //因为如果一开始就为NULL，对应的是index为0的情况
    //此情况下程序不会进入循环，也就不会对tempNode进行访问
    //如果一开始不为NULL，因为二项队列中的树一定是完全树
    //所以从index - 1到0的下标对应的树均存在，在循环时tempNode也就不会为NULL（除了最后跳出循环时）
    for(i = index - 1; i >= 0; i--){
         newQueue->theTrees[i] = tempNode;
         tempNode = tempNode->nextSibling;
         newQueue->theTrees[i]->nextSibling = NULL;
    }
    merge(queue, newQueue);
}
```

-----

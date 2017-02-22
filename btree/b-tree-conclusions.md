## B-Tree

### Index

-----

+ [Basic](#basic)
+ [Insert](#insert)
+ [Delete](#delete)

-----

### Basic

-----

B-Tree有两个比较重要的性质：

+ 所有的leaf均在同一个level上
+ 除了root之外，其它所有node中所储存的数据至少为Minimum Degree - 1，至多为Minimum Degree * 2 - 1

两个结构定义，其中```struct TreeNode```定义了B-Tree中的一个node，而```struct Tree```则是包含了一个指向B-Tree的root的指针，相当于一个dummy node，设置这个结构的目的是为了可以将其作为参数传入函数中，以便可以在函数中修改root节点的值。

```C
typedef struct TreeNode *PtrBTNode;
typedef struct TreeNode BTNode;
struct TreeNode{
    int Num;
    Bool IsLeaf;
    PtrElementType Key;
    PtrBTNode *Child;
};

typedef struct Tree *PtrBT;
struct Tree{
    PtrBTNode Root;
};
```

其他的一些定义和声明，其中```ShiftKey```函数用于对每个node中的```Key```数组进行部分左移或右移，```Direction```控制左移或右移方向（1-右移，0-左移），而```Begin```和```End```参数则是用来传递被移动部分的最左端和最右端的元素下标，```ShiftChild```函数同理针对```Child```数组；```GetIndex```函数则是用来搜索在一个节点中大于或等于```Val```的最小元素的下标，如果均小于```Val```，就返回数组的大小（就是刚好越界了）：

```C
#define MinDegree 3

typedef int ElementType;
typedef int* PtrElementType;

typedef enum BoolType Bool;
enum BoolType{
    False = 0,
    True = 1
};

void ShiftKey(PtrElementType Key, Bool Direction, int Begin, int End){
    int i;
    
    if(True == Direction){
        for(i = End; i >= Begin; i--){
            Key[i + 1] = Key[i];
        }
    }
    else{
        for(i = Begin; i <= End; i++){
            Key[i - 1] = Key[i];
        }
    }
}

void ShiftChild(PtrBTNode *Child, Bool Direction, int Begin, int End){
    int i;
    
    if(True == Direction){
        for(i = End; i >= Begin; i--){
            Child[i + 1] = Child[i];
        }
    }
    else{
        for(i = Begin; i <= End; i++){
            Child[i - 1] = Child[i];
        }
    }
}

int GetIndex(PtrElementType Key, int Size, ElementType Val){
    int i;
    
    for(i = 0; i < Size; i++){
        if(Key[i] >= Val){
            break;
        }
    }
    
    return i;
}

void BTPrintTree(PtrBTNode Root){
    int i;
    
    if(NULL == Root){
        return;
    }
    
    putchar('[');
    for(i = 0; i < Root->Num; i++){
        printf("%d", Root->Key[i]);
        if(i != Root->Num - 1){
            putchar(' ');
        }
    }
    putchar(']');
    printf("%d", Root->IsLeaf);
    putchar('\n');
    
    for(i = 0; i <= Root->Num; i++){
        BTPrintTree(Root->Child[i]);
    }
}

void BTCreateTree(PtrBT T){
    int i;
    //A test case
    int a[] = {12,1,9,2,0,11,7,19,4,15,18,5,14,13,10,16,6,3,8,17,20,21,23};
    
    for(i = 0; i < 23; i++){
        BTInsert(T, a[i]);
        BTPrintTree(T->Root);
        printf("The End\n");
    }
}
```
-----

<span id = "jump01">
### Search
</span>

-----

搜索节点的过程比较简单，下面是函数代码：

```C
PtrBTNode BTSearch(PtrBTNode Root, ElementType Val, int* Index){
    int i;
    
    for(i = 0; i < Root->Num&&Val > Root->Key[i]; i++){
        ;
    }
    if(i < Root->Num&&Root->Key[i] == Val){
        *Index = i;
        return Root;
    }
    else if(True == Root->IsLeaf){
        return NULL;
    }
    else{
        return BTSearch(Root->Child[i], Val, Index);
    }
}
```

-----

### Insert

-----

在B-Tree中插入一个值，为了不破坏B-Tree的第一个性质，必须将其插入在leaf中，但是又因为B-Tree同时具有第二个性质，所以要保证待插入值一定能成功插入，必须使得所有节点都是非满的，因此在某一个节点成为满节点时，就必须将该节点分裂为两个节点（**分裂节点：**将满节点分裂为两个包含Minimum Degree - 1个数据的节点，并将原来满节点中数据的中值插入原满节点的父节点中，同时在原满节点的父节点中插入指向两个分裂所得节点的指针，当然实际上只需要插入一个新增指针）。虽然只要leaf保持非满就一定能成功插入，但是当leaf成为满节点分裂时，会有一个数据向上插入到其父节点中，所以必须保证其父节点也非满，如果父节点被插入后成为满节点还需要继续分裂并向上插入数据，依此类推，相当于所有节点都需要保持非满状态。

在插入时向上调整的过程中看似需要递归回溯，其实可以在向下搜索插入位置时，对于即将搜索的下一个节点，都检查其是否为满节点，如果是满节点就将其分裂。而又因为对于搜索路径上的每个节点都进行判断，所以搜索路径上已经搜索过的节点一定是非满的，那么即将搜索到的节点如果需要分裂，分裂后向上插入的那个数据就一定能够插入成功。综上所述，这样的处理方式一定能够成功执行，并且可以保证数据一定能够插入成功。

<span id = "jump021">
**分裂节点**
</span>

这个函数传入的是指向待分裂节点的父节点的指针和指向待分裂节点的指针在其父节点Child数组中的下标。

首先用```BTAllocateNode```函数新建一个节点```NewNode```，将待分裂节点的Minimum Degree - 1个数据和Minimum Degree个Child指针转移到```NewNode```中，同时应当注意```NewNode```和待分裂节点应该处于同一层，***即两者```Isleaf```成员的值应当保持一致(即下面代码段中的Caution注释行)***，再将待分裂节点的父节点中```Key```数组和```Child```数组的部分元素向右移动，为之后的插入腾出位置，最后将待分裂节点中数据的中值插入其父节点中，并将指向```NewNode```的指针也插入待分裂节点的父节点中。

```C
void BTChildSplit(PtrBTNode SplitNodeP, int ChildIndex){
    int i;
    PtrBTNode NewNode = BTAllocateNode();
    PtrBTNode FullNode = SplitNodeP->Child[ChildIndex];
    
    for(i = 0; i < MinDegree - 1; i++){
        NewNode->Key[i] = FullNode->Key[MinDegree + i];
    }
    if(False == FullNode->IsLeaf){
        NewNode->IsLeaf = False;//Caution!
        for(i = 0; i < MinDegree; i++){
            NewNode->Child[i] = FullNode->Child[MinDegree + i];
        }
    }
    NewNode->Num = FullNode->Num = MinDegree - 1;
    
    ShiftKey(SplitNodeP->Key, 1, ChildIndex, SplitNodeP->Num - 1);
    SplitNodeP->Key[ChildIndex] = FullNode->Key[MinDegree - 1];
    ShiftChild(SplitNodeP->Child, 1, ChildIndex + 1, SplitNodeP->Num);
    SplitNodeP->Child[ChildIndex + 1] = NewNode;
    (SplitNodeP->Num)++;
}

PtrBTNode BTAllocateNode(){
    int i;
    PtrBTNode NewNode = (PtrBTNode)malloc(sizeof(BTNode));
    
    NewNode->Num = 0;
    NewNode->IsLeaf = True;
    NewNode->Key = (PtrElementType)malloc(sizeof(ElementType) * (MinDegree * 2 - 1));
    NewNode->Child = (PtrBTNode*)malloc(sizeof(PtrBTNode) * (MinDegree * 2));
    for(i = 0; i < MinDegree * 2; i++){
        NewNode->Child[i] = NULL;
    }
    
    return NewNode;
}
```

<span id = "jump022">
**插入节点**
</span>

```BTInsertNonFull```函数首先找到合适的插入位置，如果当前节点是leaf则直接插入，如果不是则继续向下搜索相应位置的子树（即包含最终有数据插入的leaf的子树），当然在搜索之前需要判断即将搜索的节点是否为满节点，为满节点则将其分裂，因为分裂后原来的满节点会向上插入一个数据，**所以就需要重新判断新的向下搜索位置(即下面代码段中Caution注释行)**。

```BTInsert```则是因为```BTInsertNonFull```函数并不能处理插入时root为满节点的情况，所以需要这个函数来判断，当root为满节点时新建一个root节点，再将原root节点分裂，这里有两个需要注意的地方，第一是在分裂开始前新建的root节点的```Isleaf```成员值需要改为```False```，并且新建的root节点的```Num```成员值虽然还是0，但```Child```数组的首元素应当指向原root节点，第二是结构```T```中的```root```指针需要更改。


```C
void BTInsertNonFull(PtrBTNode CurrentNode, ElementType Val){
    int Index = GetIndex(CurrentNode->Key, CurrentNode->Num, Val);
    
    if(True == CurrentNode->IsLeaf){
        
        ShiftKey(CurrentNode->Key, 1, Index, CurrentNode->Num - 1);
        CurrentNode->Key[Index] = Val;
        (CurrentNode->Num)++;
        
    }
    else{
        
        if(MinDegree * 2 - 1 == CurrentNode->Child[Index]->Num){
            BTChildSplit(CurrentNode, Index);
            if(CurrentNode->Key[Index] < Val){//Caution
                Index++;
            }
        }
        BTInsertNonFull(CurrentNode->Child[Index], Val);
    }
}

void BTInsert(PtrBT T, ElementType Val){
    PtrBTNode NewNode;
    
    if(MinDegree * 2 - 1 == T->Root->Num){
        NewNode = BTAllocateNode();
        NewNode->IsLeaf = False;
        NewNode->Child[0] = T->Root;
        T->Root = NewNode;
        
        BTChildSplit(NewNode, 0);
    }
    
    BTInsertNonFull(T->Root, Val);
}
```

-----

### Delete

-----

删除操作稍微有些复杂，因为B-Tree具有的第二个性质，所以必须保证一个节点在删除一个数据之后数据量不小于Minimum Degree - 1，因此类似于插入的过程，在删除时，对于即将搜索到的下一个节点，如果在搜索之前该节点的数据量已经为Minimum Degree - 1，则考虑从兄弟节点“借”一个数据填充，若其兄弟节点也只剩Minimum Degree - 1个数据，则考虑和兄弟节点合并；对于需要有数据被删除的节点，如果是叶节点则直接删除即可，如果不是叶节点，因为单纯的删除数据会使得```Child```数组的Size比```Key```数组的大2，不符合B-Tree的要求，因此考虑从前驱或者后继节点找一个数据填充到被删除数据的位置上，然后再递归地删除前驱或者后继节点中被用来填充的数据，如果前驱或后继节点的数据量均只剩Minimum Degree - 1，则考虑合并前驱和后继节点，然后在新的节点中递归删除（具体的在后面）。

<span id = "jump031">
**合并节点**
</span>

函数传入的参数是待合并节点的父节点和指向两个待合并节点的指针在其父节点```Child```数组中的下标，函数先将右边待合并节点中的```Key```和```Child```数组依次赋值给左边待合并节点，当然左边待合并节点的```Key```数组的中点位置需要空出，因为合并后待合并节点的父节点的```Child```数组少了一个元素，相应的```Key```数组也应该减少一个元素，因此将两个待合并节点在其父节点中所夹的数据移动到左边待合并节点的```Key```数组中点位置，然后将待合并节点的父节点的```Key```和```Child```数组中的部分元素向左移动，合并完成。**不过这里需要注意的是如果是root的子节点需要合并并且合并后root中无数据，这样的情况下就应该更改结构```T```中```root```指针的值(即下面程序中Caution注释行)**。

```C
void Merge(PtrBT T, PtrBTNode ParentNode, int LeftIndex, int RightIndex){
    PtrBTNode LeftNode = ParentNode->Child[LeftIndex], RightNode = ParentNode->Child[RightIndex];
    int i;
    
    for(i = 0; i < MinDegree - 1; i++){
        LeftNode->Key[MinDegree + i] = RightNode->Key[i];
    }
    if(False == LeftNode->IsLeaf){
        for(i = 0; i < MinDegree; i++){
            LeftNode->Child[MinDegree + i] = RightNode->Child[i];
        }
    }
    LeftNode->Key[MinDegree - 1] = ParentNode->Key[LeftIndex];
    LeftNode->Num = MinDegree * 2 - 1;
    
    ShiftKey(ParentNode->Key, 0, LeftIndex + 1, ParentNode->Num - 1);
    ShiftChild(ParentNode->Child, 0, RightIndex + 1, ParentNode->Num);
    (ParentNode->Num)--;
    
    if(ParentNode == T->Root && 0 == ParentNode->Num){//Caution
        T->Root = LeftNode;
    }
}
```

<span id = "jump032">
**删除节点**
</span>

删除节点有很多情况：

1. 待删除数据```Val```在当前节点中

	1.1 如果当前节点是leaf则直接删除
	
	1.2 如果当前节点不是leaf则检查夹着```Val```的两个子节点
	
	   + 如果左子节点的数据数量大于Minimum Degree - 1，则用左子节点中最大的数据来代替```Val```，递归地删除左子节点的最大数据
	   
	   + 如果右子节点的数据数量大于Minimum Degree - 1，则用右子节点中最小的数据来代替```Val```，递归地删除右子节点的最小数据
	   
	   + 如果以上两种情况均不符合则合并```Val```、左子节点和右子节点，在合并后的新节点中递归地删除```Val```
		
2. 待删除数据```Val```不在当前节点中

	2.1 如果当前节点是leaf则表示B-Tree中没有该数据
	
	2.2 如果当前节点不是叶节点，当前节点中大于```Val```的数据中最小值的下标为Index，假设当前节点的```Child[Index]```所指节点为SubNode，其左兄弟为LeftNode，右兄弟为RightNode，**需要注意这里的Index可能等于当前节点的数据量大小**
	
	   + 如果SubNode中的数据量大于Minimum Degree - 1，则直接在SubNode中递归删除Val
	   
	   + 如果SubNode中的数据量小于等于Minimum Degree - 1（小于时为root），则（**以下情况均需要考虑LeftNode和RightNode是否存在(即下面代码中的Caution注释行)**）
	   
	   	   + 如果LeftNode中的数据量大于Minimum Degree - 1，将```Val```左边的数据插入SubNode中，再用LeftNode中最大的数据来填入```Val```左边的数据的空缺中，并将LeftNode中最右端的子节点移动到SubNode中，然后在SubNode中递归删除```Val```
	   	   
	   	   + 如果RightNode中的数据量大于Minimum Degree - 1，将```Val```插入SubNode中，再用RightNode中最小的数据来填入```Val```的空缺中，并将RightNode中最左端的子节点移动到SubNode中，然后在SubNode中递归删除```Val```
	   	   
	   	   + 如果以上两种情况均不符合，则将LeftNode或RightNode和SubNode合并，然后在新的节点中递归的删除```Val```

```C
void BTDelete(PtrBT T, PtrBTNode CurrentNode, ElementType Val){
    int Index;
    PtrBTNode Precursor, Successor, SubNode;
    
    Index = GetIndex(CurrentNode->Key, CurrentNode->Num, Val);
    SubNode = CurrentNode->Child[Index];
    
    if(Index < CurrentNode->Num && CurrentNode->Key[Index] == Val){
        
        if(True == CurrentNode->IsLeaf){
            ShiftKey(CurrentNode->Key, 0, Index + 1, CurrentNode->Num - 1);
            (CurrentNode->Num)--;
            return;
        }
        
        else{
            
            Precursor = CurrentNode->Child[Index];
            Successor = CurrentNode->Child[Index + 1];
            
            if(Precursor->Num > MinDegree - 1){
                CurrentNode->Key[Index] = Precursor->Key[Precursor->Num - 1];
                BTDelete(T, Precursor, Precursor->Key[Precursor->Num - 1]);
            }
            else if(Successor->Num > MinDegree - 1){
                CurrentNode->Key[Index] = Successor->Key[0];
                BTDelete(T, Successor, Successor->Key[0]);
            }
            else{
                Merge(T, CurrentNode, Index, Index + 1);
                BTDelete(T, CurrentNode->Child[Index], Val);
            }
        }
    }
    else{
        if(True == CurrentNode->IsLeaf){
            return;
        }
        else{
            
            if(SubNode->Num > MinDegree - 1){
                BTDelete(T, SubNode, Val);
            }
            
            else{
                //Caution
                if(Index > 0){
                    Precursor = CurrentNode->Child[Index - 1];
                }
                if(Index < CurrentNode->Num){
                    Successor = CurrentNode->Child[Index + 1];
                }
                
                if(Index > 0 && Precursor->Num > MinDegree - 1){
                    
                    ShiftKey(SubNode->Key, 1, 0, SubNode->Num - 1);
                    ShiftChild(SubNode->Child, 1, 0, SubNode->Num);
                    SubNode->Key[0] = CurrentNode->Key[Index - 1];
                    SubNode->Child[0] = Precursor->Child[Precursor->Num];
                    (SubNode->Num)++;
                    
                    CurrentNode->Key[Index - 1] = Precursor->Key[Precursor->Num - 1];
                    (Precursor->Num)--;
                    
                    BTDelete(T, SubNode, Val);
                    
                }
                
                else if(Index < CurrentNode->Num && Successor->Num > MinDegree - 1){
                    
                    SubNode->Key[SubNode->Num] = CurrentNode->Key[Index];
                    SubNode->Child[SubNode->Num + 1] = Successor->Child[0];
                    (SubNode->Num)++;
                    
                    CurrentNode->Key[Index] = Successor->Key[0];
                    
                    ShiftKey(Successor->Key, 0, 1, Successor->Num - 1);
                    ShiftChild(Successor->Child, 0, 1, Successor->Num);
                    (Successor->Num)--;
                    
                    BTDelete(T, CurrentNode->Child[Index], Val);
                    
                }
                
                else{
                    if(Index > 0){
                        Merge(T, CurrentNode, Index - 1, Index);
                        BTDelete(T, Precursor, Val);
                    }
                    else{
                        Merge(T, CurrentNode, Index, Index + 1);
                        BTDelete(T, SubNode, Val);
                    }
                }
            }
        }
    }
}
```
-----
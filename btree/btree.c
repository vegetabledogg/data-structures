#include <stdio.h>
#include <stdlib.h>

#define MinDegree 3

typedef int ElementType;
typedef int* PtrElementType;

typedef enum BoolType Bool;
enum BoolType{
    False = 0,
    True = 1
};

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

PtrBTNode BTAllocateNode();
PtrBTNode BTSearch(PtrBTNode Root, ElementType Val, int* Index);
void BTChildSplit(PtrBTNode SplitNodeP, int ChildIndex);
void BTInsertNonFull(PtrBTNode Root, ElementType Val);
void BTInsert(PtrBT T, ElementType Val);
void BTDelete(PtrBT T, PtrBTNode Root, ElementType Val);
void Merge(PtrBT T, PtrBTNode ParentNode, int LeftIndex, int RightIndex);
void ShiftKey(PtrElementType Key, Bool Direction, int Begin, int End);
void ShiftChild(PtrBTNode *Child, Bool Direction, int Begin, int End);
int GetIndex(PtrElementType Key, int Size, ElementType Val);
void BTPrintTree(PtrBTNode Root);
void BTCreateTree(PtrBT T);

int main(){
    PtrBT T = (PtrBT)malloc(sizeof(struct Tree));
    
    T->Root = BTAllocateNode();
    BTCreateTree(T);
    
    printf("B_Tree after delete 16:\n");
    BTDelete(T, T->Root, 16);
    BTPrintTree(T->Root);
    printf("B_Tree after delete 18:\n");
    BTDelete(T, T->Root, 18);
    BTPrintTree(T->Root);
    printf("B_Tree after delete 20:\n");
    BTDelete(T, T->Root, 20);
    BTPrintTree(T->Root);
    printf("B_Tree after delete 19:\n");
    BTDelete(T, T->Root, 19);
    BTPrintTree(T->Root);
    printf("B_Tree after delete 0:\n");
    BTDelete(T, T->Root, 0);
    BTPrintTree(T->Root);
    printf("B_Tree after delete 5:\n");
    BTDelete(T, T->Root, 5);
    BTPrintTree(T->Root);
    printf("B_Tree after delete 2:\n");
    BTDelete(T, T->Root, 2);
    BTPrintTree(T->Root);
    
    return 0;
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

void BTChildSplit(PtrBTNode SplitNodeP, int ChildIndex){
    int i;
    PtrBTNode NewNode = BTAllocateNode();
    PtrBTNode FullNode = SplitNodeP->Child[ChildIndex];
    
    for(i = 0; i < MinDegree - 1; i++){
        NewNode->Key[i] = FullNode->Key[MinDegree + i];
    }
    if(False == FullNode->IsLeaf){
        NewNode->IsLeaf = False;
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
            if(CurrentNode->Key[Index] < Val){
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
    
    if(ParentNode == T->Root && 0 == ParentNode->Num){
        T->Root = LeftNode;
    }
}

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
    int a[] = {12,1,9,2,0,11,7,19,4,15,18,5,14,13,10,16,6,3,8,17,20,21,23};
    
    for(i = 0; i < 23; i++){
        BTInsert(T, a[i]);
        BTPrintTree(T->Root);
        printf("The End\n");
    }
}

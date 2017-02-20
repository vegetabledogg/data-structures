#include <stdio.h>
#include <stdlib.h>

#define ElementType int

typedef struct Node *PtrLHeapNode;
struct Node{
	ElementType key;
	PtrLHeapNode left;
	PtrLHeapNode right;
	int npl;
};

int Min(int a, int b);
PtrLHeapNode AllocateNode(ElementType val);
int GetNpl(PtrLHeapNode node);
void SwapChild(PtrLHeapNode node);
void FixHeap(PtrLHeapNode node);
PtrLHeapNode MergeHeap(PtrLHeapNode heap_1, PtrLHeapNode heap_2);
PtrLHeapNode InsertNode(PtrLHeapNode root, ElementType val);
PtrLHeapNode DeleteMax(PtrLHeapNode root);
void PrintHeap(PtrLHeapNode root);
void Preorder(PtrLHeapNode root);
void Inorder(PtrLHeapNode root);

int main(){
	PtrLHeapNode root = NULL;
	int val, times, round = 1;

	printf("Enter keys to be inserted and end with -1\n");
	while(1){
		scanf("%d", &val);
		if(-1 == val){
			break;
		}
		root = InsertNode(root, val);
	}
	printf("How many keys do you want to delete from the heap:\n");
	scanf("%d", &times);
	while(times){
		printf("Round %d:\n", round);
		root = DeleteMax(root);
		PrintHeap(root);
		times--;
		round++;
	}
	return 0;
}

int Min(int a, int b){
	return a > b ? b : a;
}

int GetNpl(PtrLHeapNode node){
	if(NULL == node){
		return -1;
	}
	return Min(GetNpl(node->left), GetNpl(node->right)) + 1;
}

PtrLHeapNode AllocateNode(ElementType val){
	PtrLHeapNode newNode = (PtrLHeapNode)malloc(sizeof(struct Node));

	newNode->key = val;
	newNode->left = newNode->right = NULL;
	newNode->npl = GetNpl(newNode);

	return newNode;
}

void SwapChild(PtrLHeapNode node){
	PtrLHeapNode tempNode = node->left;

	node->left = node->right;
	node->right = tempNode;
}

void FixHeap(PtrLHeapNode node){
	if(GetNpl(node->left) < GetNpl(node->right)){
		SwapChild(node);
	}
}

PtrLHeapNode MergeHeap(PtrLHeapNode heap_1, PtrLHeapNode heap_2){
	if(NULL == heap_1){
		return heap_2;
	}
	else if(NULL == heap_2){
		return heap_1;
	}

	if(heap_1->key > heap_2->key){
		heap_1->right = MergeHeap(heap_1->right, heap_2);
		FixHeap(heap_1);
		return heap_1;
	}
	else{
		heap_2->right = MergeHeap(heap_1, heap_2->right);
		FixHeap(heap_2);
		return heap_2;
	}
}

PtrLHeapNode InsertNode(PtrLHeapNode root, ElementType val){
	PtrLHeapNode newNode = AllocateNode(val);

	return MergeHeap(root, newNode);
}

PtrLHeapNode DeleteMax(PtrLHeapNode root){
	if(NULL == root){
		printf("ERROR\n");
		return NULL;
	}

	return MergeHeap(root->left, root->right);
}

void PrintHeap(PtrLHeapNode root){
	printf("Preorder traversal:\n");
	Preorder(root);
	printf("Inorder traversal:\n");
	Inorder(root);
	putchar('\n');
}

void Preorder(PtrLHeapNode root){
	if(NULL == root){
		return;
	}
	printf("%d ", root->key);
	Preorder(root->left);
	Preorder(root->right);
}

void Inorder(PtrLHeapNode root){
	if(NULL == root){
		return;
	}
	Inorder(root->left);
	printf("%d ", root->key);
	Inorder(root->right);
}

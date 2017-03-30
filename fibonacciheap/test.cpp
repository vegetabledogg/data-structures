#include <iostream>
#include "fibheap.h"
using namespace std;

#define DEBUG 0

int a[] = {12,  7, 25, 15, 28,
           33, 41,  1};

int b[] = {18, 35, 20, 42,  9,
           31, 23,  6, 48, 11,
           24, 52, 13,  2};

void testBasic()
{
    int i;
    int blen = sizeof(b) / sizeof(b[0]);
    FibHeap *hb = new FibHeap();

    cout << "== 斐波那契堆(hb)中依次添加: ";
    for(i = 0; i < blen; i++)
    {
        cout << b[i] << " ";
        hb->fibHeapInsert(b[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(hb)删除最小节点" << endl;
    hb->fibHeapExtractMin();
    hb->print();
}

void testInsert()
{
    int i;
    int alen = sizeof(a) / sizeof(a[0]);
    FibHeap* ha = new FibHeap();

    cout << "== 斐波那契堆(ha)中依次添加: ";
    for(i = 0; i < alen; i++)
    {
        cout << a[i] <<" ";
        ha->fibHeapInsert(a[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(ha)删除最小节点" << endl;
    ha->fibHeapExtractMin();
    ha->print();

    cout << "== 插入50" << endl;
    ha->fibHeapInsert(50);
    ha->print();
}

void testUnion()
{
    int i;
    int alen = sizeof(a) / sizeof(a[0]);
    int blen = sizeof(b) / sizeof(b[0]);
    FibHeap *ha = new FibHeap();
    FibHeap *hb = new FibHeap();

    cout << "== 斐波那契堆(ha)中依次添加: ";
    for(i = 0; i < alen; i++)
    {
        cout << a[i] <<" ";
        ha->fibHeapInsert(a[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(ha)删除最小节点" << endl;
    ha->fibHeapExtractMin();
    ha->print();

    cout << "== 斐波那契堆(hb)中依次添加: ";
    for(i = 0; i < blen; i++)
    {
        cout << b[i] <<" ";
        hb->fibHeapInsert(b[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(hb)删除最小节点" << endl;
    hb->fibHeapExtractMin();
    hb->print();

    cout << "== 合并ha和hb" << endl;
    ha->fibHeapUnion(hb);
    ha->print();
}

void testfibHeapExtractMin()
{
    int i;
    int alen = sizeof(a) / sizeof(a[0]);
    int blen = sizeof(b) / sizeof(b[0]);
    FibHeap *ha = new FibHeap();
    FibHeap *hb = new FibHeap();

    cout << "== 斐波那契堆(ha)中依次添加: ";
    for(i = 0; i < alen; i++)
    {
        cout << a[i] <<" ";
        ha->fibHeapInsert(a[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(ha)删除最小节点" << endl;
    ha->fibHeapExtractMin();

    cout << "== 斐波那契堆(hb)中依次添加: ";
    for(i = 0; i < blen; i++)
    {
        cout << b[i] <<" ";
        hb->fibHeapInsert(b[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(hb)删除最小节点" << endl;
    hb->fibHeapExtractMin();

    cout << "== 合并ha和hb" << endl;
    ha->fibHeapUnion(hb);
    ha->print();


    cout << "== 删除最小节点" << endl;
    ha->fibHeapExtractMin();
    ha->print();
}

void testDecrease()
{
    int i;
    int blen = sizeof(b) / sizeof(b[0]);
    FibHeap* hb = new FibHeap();

    cout << "== 斐波那契堆(hb)中依次添加: ";
    for(i = 0; i < blen; i++)
    {
        cout << b[i] <<" ";
        hb->fibHeapInsert(b[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(hb)删除最小节点" << endl;
    hb->fibHeapExtractMin();
    hb->print();

    cout << "== 将20减小为2" << endl;
    hb->fibHeapDecreaseKey(20, 2);
    hb->print();
}

void testDelete()
{
    int i;
    int blen = sizeof(b) / sizeof(b[0]);
    FibHeap *hb = new FibHeap();

    cout << "== 斐波那契堆(hb)中依次添加: ";
    for(i = 0; i < blen; i++)
    {
        cout << b[i] <<" ";
        hb->fibHeapInsert(b[i]);
    }
    cout << endl;
    cout << "== 斐波那契堆(hb)删除最小节点" << endl;
    hb->fibHeapExtractMin();
    hb->print();

    cout << "== 删除节点20" << endl;
    hb->fibHeapDelete(20);
    hb->print();
}

int main()
{
    testBasic();
    testInsert();
    testUnion();
    testfibHeapExtractMin();
    testDecrease();
    testDelete();

    return 0;
}

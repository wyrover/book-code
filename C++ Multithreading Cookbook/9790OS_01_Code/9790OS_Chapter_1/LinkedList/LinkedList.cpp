#include <iostream>

using namespace std;

#include "CQueue.h"
#include "CStack.h"

int _main()
{
    CQueue<int>* cQueue = new CQueue<int>();
    CStack<double>* cStack = new CStack<double>();

    for (int i = 0; i < 10; i++) {
        cQueue->Enqueue(new int(i));
        cStack->Push(new double(i / 10.0));
    }

    cout << "Queue - integer collection:" << endl;

    for (; cQueue->Count();) {
        cout << *cQueue->Dequeue() << " ";
    }

    cout << endl << endl << "Stack - double collection:" << endl;

    for (; cStack->Count();) {
        cout << *cStack->Pop() << " ";
    }

    delete cQueue;
    delete cStack;
    cout << endl << endl;
    return system("pause");
}

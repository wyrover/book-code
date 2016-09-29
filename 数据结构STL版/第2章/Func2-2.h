//Func2-2.h 验证队列类成员函数的主程序
void main()
{
    bool i;
    T d;

    for (int k = 0; k < 5; k++)
        assert(Q.EnQueue(k));

    cout << "入队5个元素后，队列的元素依次为";
    Q.QueueTraverse(print);
    cout << "是否空队列？" << boolalpha << Q.QueueEmpty();
    cout << "。队列的长度为" << Q.QueueLength() << endl;
    Q.DeQueue(d);
    cout << "出队了队头元素，其值为" << d;
    i = Q.GetHead(d);

    if (i == true)
        cout << "。新的队头元素是" << d << endl;

    Q.ClearQueue();
    cout << "清空队列后，是否空队列？" << boolalpha << Q.QueueEmpty();
    cout << "。队列的长度为" << Q.QueueLength() << endl;
}


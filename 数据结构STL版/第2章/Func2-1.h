//Func2-1.h 验证栈类成员函数的主程序
void main()
{
    int j;
    T e;

    for (j = 1; j <= 5; j++)
        S.Push(j);

    cout << "栈中元素由栈底到栈顶依次为";
    S.StackTraverse(print);
    S.Pop(e);
    cout << "弹出的栈顶元素e=" << e;
    cout << "。栈空否？" << boolalpha << S.StackEmpty() << endl;
    S.GetTop(e);
    cout << "新的栈顶元素e=" << e << "，栈的长度为" << S.StackLength() << endl;
    S.ClearStack();
    cout << "清空栈后，栈空否？" << boolalpha << S.StackEmpty() << endl;
}


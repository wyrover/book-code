//Func4-4.h 增加的I/O操作
void VisitRB(RBTNode<T> &c)
{
    cout << '(' << c.data.key << ", ";

    if (c.RB == Red)
        cout << "Red" << ')';
    else
        cout << "Black" << ')';
}


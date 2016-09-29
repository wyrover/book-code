//Func4-1.h 数据类型为基本类型的I/O操作
void Visit(BiTNode<T> &c)
{
    cout << c.data << ' ';
}
bool InputFromFile(ifstream &f, T &c)
{
    f >> c;
    return f.good();
}
void Input(T &c)
{
    cin >> c;
}


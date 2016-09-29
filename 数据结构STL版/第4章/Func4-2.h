//Func4-2.cpp 定义数据类型和关键字类型及对它们的I/O操作
typedef int KeyType;
struct T {
    KeyType key;
    int others;
};
void Visit(BiTNode<T> &c)
{
    cout << '(' << c.data.key << ", " << c.data.others << ')';
}
bool InputFromFile(ifstream &f, T &c)
{
    f >> c.key >> c.others;
    return f.good();
}
void InputKey(KeyType &k)
{
    cin >> k;
}


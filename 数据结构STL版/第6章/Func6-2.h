//Func6-2.h 定义模板<D>的实参HD及相应的I/O操作
struct HD {
    KeyType key;
    int order;
};
void Visit(HD c)
{
    cout << '(' << c.key << ", " << c.order << ')' << endl;
}
void Visit(int i, HD c)
{
    cout << '[' << i << "]: ";
    Visit(c);
}
bool InputFromFile(ifstream &f, HD &c)
{
    f >> c.key >> c.order;
    return f.good();
}
void InputKey(KeyType &k)
{
    cin >> k;
}
void Input(HD &c)
{
    cin >> c.key >> c.order;
}


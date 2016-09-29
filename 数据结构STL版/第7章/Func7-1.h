//Func7-1.h 定义模板<D>的实参ID及相应的I/O操作
struct ID {
    KeyType key;
    int order;
};
void Visit(ID &c)
{
    cout << '(' << c.key << ", " << c.order << ')';
}
bool InputFromFile(ifstream &f, ID &c)
{
    f >> c.key >> c.order;
    return f.good();
}
void PrintKey(ID &c)
{
    cout << c.key << ' ';
}


//Func6-3.h 定义模板<D>的实参Record及相应（包括KeyType）的I/O操作
struct Record {
    KeyType key;
    int order;
};
void Out(Record c)
{
    cout << '(' << c.key << "," << c.order << ')';
}
bool InputFromFile(ifstream &f, Record &c)
{
    f >> c.key >> c.order;
    return f.good();
}
void InputKey(KeyType &k)
{
    cin >> k;
}


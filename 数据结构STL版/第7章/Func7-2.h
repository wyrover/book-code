//Func7-2.h 定义模板<T>的实参Rec及相应的I/O操作
struct Rec {
    string key;
    string translation;
};
bool InputFromFile(ifstream &f, Rec &c)
{
    f >> c.key;
    getline(f, c.translation);
    return f.good();
}
void Visit(Rec c)
{
    cout << setiosflags(ios::left) << setw(3) << c.key << c.translation << endl;
}


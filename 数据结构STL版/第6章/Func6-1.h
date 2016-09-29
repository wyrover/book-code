//Func6-1.h 定义模板<D>的实参Sc及对其的I/O操作
#define number key
struct Sc {
    long number;
    string name;
    int Chinese;
    int English;
    int math;
    int physics;
    int total;
};
void Visit(Sc &c)
{
    cout << setw(7) << c.number << setw(8) << c.name << setw(4) << c.Chinese << setw(5)
         << c.English << setw(5) << c.math << setw(5) << c.physics << setw(5) << c.total << endl;
}
void Total(Sc &c)
{
    c.total = c.Chinese + c.English + c.math + c.physics;
}
bool InputFromFile(ifstream &f, Sc &c)
{
    f >> c.number >> c.name >> c.Chinese >> c.English >> c.math >> c.physics;
    return f.good();
}
void InputKey(KeyType &k)
{
    cin >> k;
}


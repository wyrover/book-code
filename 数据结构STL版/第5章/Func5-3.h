//Func5-3.h 定义一种顶点结构模板<VerT>的实参V及对其的I/O操作
struct V {
    string name;
    int size;
};
void Visit(V ver)
{
    cout << '(' << ver.name << ", " << ver.size << ")";
}
void InputVertex(V &ver)
{
    cin >> ver.name >> ver.size;
}
void InputVertexFromFile(ifstream &f, V &ver)
{
    f >> ver.name >> ver.size;
}
void InputVertexNameFromFile(ifstream &f, V &ver)
{
    f >> ver.name;
}


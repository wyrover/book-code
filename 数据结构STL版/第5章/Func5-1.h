//Func5-1.h 定义顶点结构模板<VerT>的实参V及对其的I/O操作
struct V {
    string name;
};
void Visit(V ver)
{
    cout << ver.name << ' ';
}
void InputVertex(V &ver)
{
    cin >> ver.name;
}
void InputVertexFromFile(ifstream &f, V &ver)
{
    f >> ver.name;
}
void InputVertexNameFromFile(ifstream &f, V &ver)
{
    f >> ver.name;
}


//Func5-5.h 定义AdjListGraph类中弧结构模板<ArcT>的实参A及对其的I/O操作
struct A {
    VRType weight;
};
void InputArc(bool direct, bool weight, V ver1, V ver2, A &arc)
{
    string s = "—";

    if (direct)
        s = "→";

    if (weight) {
        cout << "请输入" << ver1.name << s << ver2.name << "的权值：";
        cin >> arc.weight;
    }
}
void OutputArc(bool direct, bool weight, V ver1, V ver2, A arc)
{
    string s = "—";

    if (direct)
        s = "→";

    cout << ver1.name << s << ver2.name;

    if (weight)
        cout << "：" << arc.weight;

    cout << ' ';
}
void InputArcFromFile(ifstream &f, A &arc)
{
}


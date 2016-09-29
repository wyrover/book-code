//Func5-6.h 定义一种AdjListGraph类中弧结构模板<ArcT>的实参A及对其的I/O操作
struct A {
    VRType weight;
    string message;
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

    cout << "请输入此弧的文字信息：";
    cin.ignore(1, '\n');
    getline(cin, arc.message);
}
void OutputArc(bool direct, bool weight, V ver1, V ver2, A arc)
{
    string s = "—";

    if (direct)
        s = "→";

    cout << ver1.name << s << ver2.name << "：(";

    if (weight)
        cout << arc.weight << ", ";

    cout << arc.message << ") ";
}
void InputArcFromFile(ifstream &f, A &arc)
{
    f.ignore(1, ' ');
    getline(f, arc.message);
}


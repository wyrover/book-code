//Main3-1.cpp 验证串HString类的成员函数
#include "C.h"
#include "HString.h"
void main()
{
    int i;
    char c, *p = "God bye!", *q = "God luck!";
    HString t, s(p), r("Good luck!");
    cout << "串s为" << s << endl;
    cout << "s[4]=" << s[4] << endl;
    cout << "串r为";
    r.Output(cout);
    HString u(r);
    cout << endl << "串u为" << u << endl;
    t.StrAssign(p);
    cout << "串t为" << t << endl;
    cout << "t的串长为" << t.size() << "，串空否？";
    cout << boolalpha << t.StrEmpty() << endl;
    s.StrAssign(q);
    cout << "串s为" << s << endl;
    i = s.StrCompare(t);

    if (i < 0)
        c = '<';
    else if (i == 0)
        c = '=';
    else
        c = '>';

    cout << "串s" << c << "串t" << endl;
    r.Concat(t, s);
    cout << "串t连接串s产生的串r为" << r << endl;
    s.StrAssign("oo");
    cout << "串s为" << s << endl;
    t.StrAssign("o");
    cout << "串t为" << t << endl;
    r.Replace(t, s);
    cout << "把串r中和串t相同的子串用串s代替后，串r为" << r << endl;
    s.ClearString();
    cout << "串s清空后，串长为" << s.size() << "，串空否？";
    cout << boolalpha << s.StrEmpty() << endl;
    s = r.substr(5, 4);
    cout << "串s为从串r的[5]起的4个字符，长度为" << s.size() << "，串s为" << s << endl;
    t.StrCopy(r);
    cout << "由串r复制得串t，串t为" << t << endl;
    t.StrInsert(5, s);
    cout << "在串t的[5]前插入串s后，串t为" << t << endl;
    t.StrDelete(0, 5);
    cout << "从串t的[0]起删除5个字符后，串t为" << t << endl;
    cout << t.Index(s, 0) << "是从串t的[0]起，和串s相同的第一个子串的起始位置" << endl;
    cout << t.Index(s, 1) << "是从串t的[1]起，和串s相同的第一个子串的起始位置" << endl;
}


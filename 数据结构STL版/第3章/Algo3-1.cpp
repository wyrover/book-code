//Algo3-1.cpp STL中的串
#include "C.h"
void main()
{
    int i;
    char c, *p = "God bye!", *q = "God luck!";
    string t, s(p), r("Good luck!");
    cout << "s=" << s << endl;
    cout << "r=" << r << endl;
    string u(r);
    cout << "u=" << u << endl;
    t.assign(p);
    cout << "t=" << t << endl;
    cout << "t的串长为" << t.size() << "，t的存储容量为" << t.capacity() << "，串空否？";
    cout << boolalpha << t.empty() << endl;
    s.assign(q);
    cout << "s=" << s << endl;
    i = s.compare(t);

    if (i < 0)
        c = '<';
    else if (i == 0)
        c = '=';
    else
        c = '>';

    cout << "串s" << c << "串t" << endl;
    r = t + s;
    cout << "r=t+s=" << r << endl;
    s = "oo";
    t = "o";
    i = -2;

    while (true) {
        i = r.find(t, i + 2);

        if (i >= 0)
            r.replace(i, 1, s);
        else
            break;
    }

    cout << "r=" << r << endl;
    s.erase();
    cout << "s清空后，串长为" << s.size() << "，串空否？";
    cout << boolalpha << s.empty() << endl;
    s = r.substr(5, 4);
    cout << "串s为从串r的[5]起的4个字符，s=" << s << endl;
    t = r;
    cout << "由串r复制得串t，t=" << t << endl;
    t.insert(5, s);
    cout << "在串t的[5]前插入串s后，串t=" << t << endl;
    t.erase(0, 5);
    cout << "从串t的[0]起删除5个字符后，t=" << t << endl;
}


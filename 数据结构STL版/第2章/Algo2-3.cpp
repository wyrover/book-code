//Algo2-3.cpp 算术表达式求值，输入负数要用(0-正数)表示
#include "C.h"
#include "SqStack.h"
template<typename T>class Evaluate
{
private:
    char Precede(char t1, char t2)
    {
        char f;

        switch (t2) {
        case'+':
        case'-':
            if (t1 == '(' || t1 == '#')
                f = '<';
            else
                f = '>';

            break;

        case'*':
        case'/':
            if (t1 == '*' || t1 == '/' || t1 == ')')
                f = '>';
            else
                f = '<';

            break;

        case'(':
            if (t1 == ')') {
                cout << "括号不匹配" << endl;
                exit(1);
            } else
                f = '<';

            break;

        case')':
            switch (t1) {
            case'(':
                f = '=';
                break;

            case'#':
                cout << "缺乏左括号" << endl;
                exit(1);

            default:
                f = '>';
            }

            break;

        case'#':
            switch (t1) {
            case'#':
                f = '=';
                break;

            case'(':
                cout << "缺乏右括号" << endl;
                exit(1);

            default:
                f = '>';
            }
        }

        return f;
    }
    bool In(char c)
    {
        switch (c) {
        case'+':
        case'-':
        case'*':
        case'/':
        case'(':
        case')':
        case'#':
            return true;

        default:
            return false;
        }
    }
    T Operate(T a, char theta, T b)
    {
        switch (theta) {
        case'+':
            return a + b;

        case'-':
            return a - b;

        case'*':
            return a * b;

        default:
            return a / b;
        }
    }
public:
    T Expression(string FileName, bool flag)
    {
        SqStack<char>OPTR(10);
        SqStack<T>OPND(10);
        string s;
        ifstream fin(FileName.c_str());
        fin >> s;
        T a, b, d;
        int i = 0;
        char c, x;
        OPTR.Push('#');

        if (flag)
            cout << "不出现负号的整型算术表达式及运算结果：" << s;
        else
            cout << "可以出现负号和小数的算术表达式及运算结果：" << s;

        s = s + '#';
        c = s[i++];
        OPTR.GetTop(x);

        while (c != '#' || x != '#') {
            if (In(c)) {
                if (!flag)
                    if (c == '-' && i == 1 || c == '(' && s[i] == '-')
                        OPND.Push(0);

                switch (Precede(x, c)) {
                case'<':
                    OPTR.Push(c);

                    if (i < s.length())
                        c = s[i++];

                    break;

                case'=':
                    OPTR.Pop(x);

                    if (i < s.length())
                        c = s[i++];

                    break;

                case'>':
                    OPTR.Pop(x);
                    OPND.Pop(b);
                    OPND.Pop(a);
                    d = Operate(a, x, b);
                    OPND.Push(d);
                }
            } else {
                if (c >= '0' && c <= '9') {
                    d = 0;

                    while (c >= '0' && c <= '9') {
                        d = d * 10 + c - '0';

                        if (i < s.length()) {
                            c = s[i++];

                            if (!flag)
                                if (c == '.') {
                                    int n = 10;
                                    c = s[i++];

                                    while (c >= '0' && c <= '9') {
                                        d = d + ((double)(c - '0')) / n;
                                        n = n * 10;

                                        if (i < s.length())
                                            c = s[i++];
                                    }
                                }
                        }
                    }

                    OPND.Push(d);
                } else {
                    cout << "出现非法字符" << endl;
                    exit(1);
                }
            }

            OPTR.GetTop(x);
        }

        OPND.Pop(a);

        if (!OPND.StackEmpty()) {
            cout << "表达式不正确" << endl;
            exit(1);
        }

        return a;
    }
};
void main()
{
    Evaluate<int> eint;
    Evaluate<double> edouble;
    cout << '=' << eint.Expression("F2-1.txt", true) << endl;
    cout << '=' << edouble.Expression("F2-2.txt", false) << endl;
}


//HString.h ×Ö·û´®µÄÀà£¨HStringÀà£©
#ifndef _HSTRING_H_
#define _HSTRING_H_
class HString
{
private:
    char *ch;
    int length;
public:
    HString()
    {
        ch = NULL;
        length = 0;
    }
    HString(const char* str)
    {
        length = strlen(str);
        ch = new char[length];
        assert(ch != NULL);

        for (int i = 0; i < length; i++)
            ch[i] = str[i];
    }
    HString(const HString &S)
    {
        length = S.length;
        ch = new char[length];
        assert(ch != NULL);

        for (int i = 0; i < length; i++)
            ch[i] = S[i];
    }
    ~HString()
    {
        ClearString();
    }
    void ClearString()
    {
        if (ch != NULL) {
            delete[]ch;
            ch = NULL;
        }

        length = 0;
    }
    void StrAssign(const char* str)
    {
        ClearString();
        length = strlen(str);

        if (length) {
            ch = new char[length];
            assert(ch != NULL);

            for (int j = 0; j < length; j++)
                ch[j] = str[j];
        }
    }
    void StrCopy(const HString &S)
    {
        ClearString();
        length = S.length;
        ch = new char[length];
        assert(ch != NULL);

        for (int i = 0; i < S.length; i++)
            ch[i] = S[i];
    }
    bool StrEmpty()const
    {
        return length == 0;
    }
    int StrCompare(const HString &S)const
    {
        for (int i = 0; i < length && i < S.length; i++)
            if (ch[i] != S[i])
                return ch[i] - S[i];

        return length - S.length;
    }
    int size()const
    {
        return length;
    }
    void Concat(const HString &S1, const HString &S2)
    {
        int i;
        ClearString();
        length = S1.length + S2.length;
        ch = new char[length];
        assert(ch != NULL);

        for (i = 0; i < S1.length; i++)
            ch[i] = S1[i];

        for (i = 0; i < S2.length; i++)
            ch[S1.length + i] = S2[i];
    }
    HString substr(int pos, int len)const
    {
        HString Sub;

        if (!(pos < 0 || pos >= length || len <= 0 || len > length - pos)) {
            Sub.length = len;
            Sub.ch = new char[len];
            assert(Sub.ch != NULL);

            for (int i = 0; i < len; i++)
                Sub.ch[i] = ch[pos + i];
        }

        return Sub;
    }
    bool StrInsert(int pos, const HString &S)
    {
        int i;

        if (pos < 0 || pos > length)
            return false;

        if (S.length) {
            char *p = new char[length + S.length];
            assert(p != NULL);

            for (i = 0; i < pos; i++)
                p[i] = ch[i];

            for (i = 0; i < S.length; i++)
                p[pos + i] = S[i];

            for (i = pos; i < length; i++)
                p[i + S.length] = ch[i];

            length += S.length;
            delete[]ch;
            ch = p;
        }

        return true;
    }
    bool StrDelete(int pos, int len)
    {
        int i;
        char *p;

        if (length < pos + len)
            return false;

        p = new char[length - len];
        assert(p != NULL);

        for (i = 0; i < pos; i++)
            p[i] = ch[i];

        for (i = pos; i < length - len; i++)
            p[i] = ch[i + len];

        length -= len;
        delete[]ch;
        ch = p;
        return true;
    }
    int Index(const HString &S, int pos)const
    {
        HString sub;

        if (pos >= 0)
            for (int i = pos; i <= length - S.length; i++) {
                sub = substr(i, S.length);

                if (sub.StrCompare(S) == 0)
                    return i;
            }

        return -1;
    }
    bool Replace(const HString &T, const HString &V)
    {
        if (T.length == 0)
            return false;

        int i = 0;

        while (i > -1) {
            i = Index(T, i);

            if (i > -1) {
                StrDelete(i, T.length);
                StrInsert(i, V);
                i += V.length;
            }
        }

        return true;
    }
    char operator[](int i)const
    {
        return ch[i];
    }
    HString& operator=(const HString &L)
    {
        if (this != &L) {
            if (ch != NULL)
                delete ch;

            ch = new char[L.length];
            assert(ch != NULL);

            for (int i = 0; i < L.length; i++)
                ch[i] = L[i];

            length = L.length;
        }

        return *this;
    }
    void Output(ostream& out)
    {
        for (int i = 0; i < length; i++)
            out << ch[i];
    }
};
ostream& operator << (ostream& out, HString& str)
{
    str.Output(out);
    return out;
}
#endif


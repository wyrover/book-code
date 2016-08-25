#ifndef CBCHEN_SEARCH

    #define CBCHEN_SEARCH
    #include "stdio.h"
    #include "string.h"
    #include "conio.h"
    #include "assert.h"

void kmp_get_next( char p [], int next [] )
{
    /*int i, j, slen;
    slen = strlen(p); 
    i = 0; 
    j = -1; 
    next[0] = -1; 
    while(i < slen) 
    { 
    if((j == -1) || (p[i] == p[j])) 
    { 
    i++; 
    j++; 
    next[i] = j; 
    } 
    else 
    { 
    j = next[j]; 
    } 
    } */
    int j, k;
    next[0] = -1;
    j = 1;

    do
    {
        k = next[j - 1];

        while (k != -1 && p[k] != p[j - 1])
            k = next[k];

        next[j++] = k + 1;
    } while (p[j] != '\0');
}

int kmp( char s [], char p [], int pos, int next [] ) //KMP模式匹配算法 
{
    int i, j, slen, plen;
    i = pos;
    j = 0;
    slen = strlen(s);
    plen = strlen(p);

    while ((i<slen) && (j<plen))
    {
        if ((j == -1) || (s[i] == p[j]))
        {
            i++;
            j++;
        }
        else
        {
            j = next[j];
        }
    }

    if (j>=plen)
    {
        return (i - plen);
    }
    else
    {
        return -1;
    }
}

int pusu_search( char s [], char p [], int pos ) //朴素的模式匹配算法 
{
    int i, j, slen, plen;
    i = pos;
    j = 0;
    slen = strlen(s);
    plen = strlen(p);

    while ((i<slen) && (j<plen))
    {
        if ((s[i] == p[j]))
        {
            i++;
            j++;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }

    if (j>=plen)
    {
        return (i - plen);
    }
    else
    {
        return -1;
    }
}

int simple_match( char *t, char *p ) //最简单的匹配算法
{
    if (t == NULL || p == NULL)
        return -1;

    int n = strlen(t), m = strlen(p), i, j, k;

    for (j = 0; j<n - m; j++)
    {
        for (i = 0; i<m && t[j + i] == p[i]; i++)
            ;

        if (i == m)
            return j;
    }

    return -1;
}

DWORD Search_NoCase( LPCSTR str, LPCSTR SubS1, LPCSTR SubS2, int iLen ) //不区分大小写查找
{
    int i;
    LPCSTR s, sdest;
    char t;

    sdest = str;

    if (!str || !SubS1 || !SubS2 || !SubS1[0] || !SubS2[0])
        return NULL;

    while (*sdest)
    {
        if ((*sdest == SubS1[0]) || (*sdest == SubS2[0]))
        {
            for (s = sdest, i = 1, s++; *s; s++, i++)
            {
                if ((*s == SubS1[i]) || (*s == SubS2[i]))
                {
                    continue;
                }

                if (i<iLen)
                    break;
                else
                    return (sdest - str);
            }
        }

        sdest++;
    }

    return -1;
}

DWORD Search_HaveCase( LPCSTR str, LPCSTR SubS, int iLen ) //区分大小写查找
{
    int i;
    LPCSTR s, sdest;
    char t;

    sdest = str;

    if (!str || !SubS || !SubS[0])
        return NULL;

    while (*sdest)
    {
        if ((*sdest == SubS[0]))
        {
            for (s = sdest, i = 1, s++; *s; s++, i++)
            {
                if ((*s == SubS[i]))
                {
                    continue;
                }

                if (i<iLen)
                    break;
                else
                    return (sdest - str);
            }
        }

        sdest++;
    }

    return -1;
}

class Search
{
    static const int MAX_CHAR_LEN = 256;
    int d[MAX_CHAR_LEN];
    int m;
    char *patt;

    public:

    Search(char *);
    int find(char *);
};

Search::Search( char *p )
{
    assert(p);

    patt = p;
    m = strlen(patt);

    int k = 0;

    for (k = 0; k<MAXCHAR; k++)
        d[k] = m;

    for (k = 0; k<m - 1; k++)
        d[patt[k]] = m - k - 1;
}

int Search::find( char *text )
{
    assert(text);

    int n = strlen(text);

    if (m>n)
        return -1;

    int k = m - 1;

    while (k<n)
    {
        int j = m - 1;
        int i = k;

        while (j>=0 && text[i] == patt[j])
        {
            j--;
            i--;
        }

        if (j == -1)
            return i + 1;

        k += d[text[k]];
    }

    return -1;
}

#endif

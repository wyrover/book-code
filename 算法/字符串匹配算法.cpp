#include<string.h>
#include<assert.h>

using namespace std;

namespace Foxhawk
{
//string searching
class BM
{
public:
 int Search(const char *textf, char *str)
 {
  init(str);
  return search(textf, strlen(textf));
 }
private:
 unsigned char skips[256];
 unsigned char search_len;
 char *searchstr;

 void init(const char *str)
 {
  search_len = strlen(str);
  int len2 = search_len + 1;

  for(int i = 0; i < 256; i++)
  {
   skips[i] = len2;
  }

  for(int i = 0; i < search_len; i++)
   skips[str[i]] = search_len - i;

  searchstr = (char*) str;
 }
 int search(const char *textf, int len)
 {
  if (search_len > len || search_len == 0)
   return 0;

  //unsigned char *end = (unsigned char*)textf + len;
  int len2 = search_len - 1;
  int j, k = len2;
  int pos = 0;

  for(;;)
  {
   j = len2;
   while (textf[pos + j] == searchstr[j] && --j >= 0);

   if(j != -1)
   {
    //unsigned char* skipindex = (unsigned char*) textf[k] + search_len;
    if(pos + search_len >= len)
     return -1;

    pos += skips[textf[pos + search_len]];
   }
   else
   {
    return pos;
   }
  }
  return -1;
 }

};

 

//不区分大小写的查找
class FastString
{
public:
 char* Search(const char *textf, char *str)
 {
  initInsens(str);
  return search(textf, strlen(textf));
 }
private:
 unsigned char searchstr[256];//模式最大长度为256
 unsigned char alfa[256]; //记住字母的位置
 unsigned char skipsInsens[256];//跳过数组
 unsigned char patlenInsens;//查找模式的长度
 
 void initInsens(const char *str)
 {
  int len1 = 0;
  
  //获得长度，进行小块拷贝
  unsigned char c = str[len1];
  while(c != '\0')
  {
   alfa[len1] = 0;
   if(c >= 'A' && c <= 'Z')
   {
    searchstr[len1] = c + 0x20;
    alfa[len1] = 1;
   }
   else
   {
    searchstr[len1] = c;
    if(c >= 'a' && c <= 'z')
     alfa[len1] = 1;
   }
   c = str[++len1];
  }

  int len2 = len1 + 1;

  //对于不在模式中的字符
  for(int i = 0; i < 255; i++)
   skipsInsens[i] = len2;

  for(int i = 0; i < len1; i++)
  {
   skipsInsens[searchstr[i]] = len1 - i;
   if(alfa[i]) skipsInsens[searchstr[i] - 0x20] = len1 - i;
  }

  patlenInsens = len1;
 }

 char * search(const char *textf, int len)
 {
  if (patlenInsens > len || patlenInsens == 0)
   return NULL;

  unsigned char *end = (unsigned char*)textf + len;
  int len2 = patlenInsens - 1;
  int j;

  for(;;)
  {
   j = len2;
   while (((textf[j] == searchstr[j]) || 
    ((alfa[j]) && (textf[j] == (char)(searchstr[j] - 0x20))))
    && --j >= 0);

   if(j != -1)
   {
    unsigned char* skipindex = (unsigned char*) textf + patlenInsens;
    if(skipindex >= end)
     return NULL;

    textf += skipsInsens[*skipindex];
   }
   else
   {
    return (char*) textf;
   }
  }
  return NULL;
 }
};

class KMP
{
public:
 int Search(const char *textf, char *str)
 {
  init(str);
  return kmp(textf);
 }

 ~KMP()
 {
  delete[] next;
 }
private:
 int *next;
 char *p;

 void init(char* str)
 {
  p = str;
  next = new int[strlen(p)];

  int j,k;
  next[0] = -1;
  j = 1;
  /*
  abcabcddes
  0111234111 
  */
  do
  {
   k = next[j-1];
   while(k != -1 && p[k] != p[j-1])
    k = next[k];
   next[j++]= k + 1;
  }while(p[j] != '\0');
 }
 int kmp(const char* t)
 {
  int k,j;
  if(*t == '\0') return 0;
  for(k = j = 0; t[k] != '\0';)
  {
   while(j != -1 && p[j] != t[k])
    j = next[j];
   k++;
   j++;
   if(p[j] == '\0')
    return k - j;
  }
  return -1;
 }
};

 

//string compare
#define macro_strcmp(s1, s2) ((s1)[0] != (s2)[0] ? (s1)[0] - (s2)[0]: strcmp(s1, s2))

inline int int_strcmp(char *s1, char *s2, int len)
{
 if((len & 0x03) != 0)
  return (macro_strcmp(s1, s2));

 for(int i = 0; *(int*)&s1[i] == *(int*)&s2[i]; i += 4)
 {
  if(i >= len)
   return(true);
 }
 return false;
}

//模糊匹配
inline char* amatch(const char* text, const char* pat, int k)
{
 int m = strlen(pat);
 assert(m-k>0);
 assert((m-k)*(k+2)<= 64);
 int j;
 __int64 Din = 0;
 __int64 M1 = 0; 
 __int64 M2 = 0; 
 __int64 M3 = 0; 
 __int64 G = 1 << k;
 int onekp1 = (1 << (k+1)) - 1;
 for (j=0; j<m-k; j++)
 {
  Din = (Din << (k+2))|onekp1;
  M1 = (M1 << (k+2))|1;
  if (j < m-k-1)
  M2 = (M2 << (k+2)) | 1;
 }
 M2=(M2<<(k+2))|onekp1;
 __int64 D=Din;
 const char* s=text;
 int c=*s++;
 while(c)
 {
  int found=0;
  const char* sp=pat;
  for(j=0;j<k+1;j++)
  {
  int cp=*sp++;
  if(c==cp)
  {
   found=1;
   break;
  }
  }
  if(found)
  {
  do
  {
   __int64 tc = 0;
   const char* sp = pat;
   for (j=0; j<m; j++)
   {
   int cp = *sp++; 
   if (c!=cp)
   c|=(1<<j);
   }
   __int64 Tc = 0;
   for (j=0; j<m-k; j++)
   Tc = (Tc<<(k+2))|((tc>>j)&onekp1);
   __int64 x = (D>>(k+2))|Tc;
   D=((D<<1)|M1)&((D<<(k+3))|M2)&(((x+M1)^x)>>1)&Din;
   if((D & G) == 0)
   return (char*)s;
   if(D != Din)
   c = *s++;
  }
  while ( D != Din && c);
 }
 if (c)
  c = *s++;
 }
 return NULL;
}  

}



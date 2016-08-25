#include <stdio.h> 
inline void Swap(char& a, char& b) 
{// 交换a和b 
    char temp = a; 
    a = b; 
    b = temp; 
} 

void Perm(char list[], int k, int m) 
{ //生成list [k：m ]的所有排列方式 
    int i; 
    if (k == m) {//输出一个排列方式 
        for (i = 0; i <= m; i++) 
            putchar(list[i]); 
        putchar('\n'); 
    } 
    else // list[k：m ]有多个排列方式 
        // 递归地产生这些排列方式 
        for (i=k; i <= m; i++) { 
            Swap (list[k], list[i]); 
            Perm (list, k+1, m); 
            Swap (list [k], list [i]); 
        } 
} 

int main() 
{ 
    char s[]="123"; 
    Perm(s, 0, 2); 
    return 0; 
} 


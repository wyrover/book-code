#include<stdio.h>

int mod(__int64 a, __int64 p, __int64 k)
{
    if (p == 1) return a % k;

    if (p % 2) return mod(a % k, p - 1, k) * a % k;
    else return mod((a * a) % k, p / 2, k);
}

int main()
{
    unsigned a, p, k;

    while (scanf("%u%u%u", &a, &p, &k) != EOF)
        printf("%d\n", mod(a, p, k));
}

#include <stdio.h>

void main()
{
    char i, j, k; /*i是a的对手，j是b的对手，k是c的对手*/

    for (i = 'x'; i<='z'; i++)
        for (j = 'x'; j<='z'; j++)
        {
            if (i != j)
                for (k = 'x'; k<='z'; k++)
                {
                    if (i != k && j != k)
                    {
                        if(i!='x'&&k!='x'&&k!='z')
   printf("order is a--%c\tb--%c\tc--%c\n",i,j,k);
                    }
                }
        }
}

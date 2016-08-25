#include<stdio.h>

void print_it(int n, int arr[])
{
    int        i;
    static int cnt = 0;
    printf("%04d:\t", ++cnt);
    for (i = 0; i < n; i++)
        printf("%2d ", arr[i]);
    printf("\n");
}


void arrange_all(int size, int arr[], int pos)
{
    int i, tmp;
    if (pos + 1== size) /*  如果已经排到最后  */
    {
        print_it(size, arr); /*  打印数组  */
        return;
    }
    for (i = pos; i < size; i++) /*  对当前位置后的所有位置排列*/
    {
        tmp = arr[pos]; /*  交换位置  */
        arr[pos] = arr[i];
        arr[i] = tmp;
        /*  递归，继续后面的调用  */
        arrange_all(size, arr, pos + 1);
        /*  在交换回来，保持原有的排列次序  */
        tmp = arr[pos];
        arr[pos] = arr[i];
        arr[i] = tmp;
    }
}



int main()
{
    int a[5] = { 6, 2, 3, 4, 5};
    arrange_all(5, a, 0);
    return 0;
}

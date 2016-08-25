#include <stdio.h>

//打印排序结果
void PrintResult(int a[], int length)
{
    for (int i = 0; i < length; i++)
        printf("%d ", a[i]);

    printf("\n");
}

//选择排序
//选择最小的数字放到数组的第一个位置，选第二小的放到第二个，依次类推
void SelectionSort(int a[], int length)
{
    for (int i = 0; i < length - 1; i++) {
        int min = a[i];

        //找到i+1开始最小的数字，赋值给min
        for (int j = i + 1; j < length; j++) {
            if (min > a[j]) {
                int temp = min;
                min = a[j];
                a[j] = temp;
            }
        }

        //如果a[i]小于min，两个数字交换
        if (min != a[i]) {
            int temp = min;
            min = a[i];
            a[i] = temp;
        }

        //打印每一次排序后的结果
        printf("Step %d:", i + 1);
        PrintResult(a, length);
    }
}

int main(int argc, char* argv[])
{
    int a[9] = {9, 4, 2, 7, 6, 5, 8, 3, 1};
    SelectionSort(a, 9);
    return 0;
}


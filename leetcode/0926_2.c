#include <stdio.h>
#include <stdlib.h>

int **generateMatrix(int n, int *returnSize, int **returnColumnSizes)
{
    // 定义二维数组
    int arr_1[n][n];
    arr_1[n][n] = n;
    int number=1;
    for (int i = 1; i <= (n - 1) / 2; i++) // 循环圈数
    {
        arr_1[i-1][i-1]=number;

    }

    int *arr = (int *)malloc(sizeof(int) * n * n); // 定义一维数组

    int **arr_p = (int **)malloc(sizeof(int *)); // 指向二维数组的指针
    *arr_p = arr;

    return arr_p;
}

int main()
{

    return 0;
}

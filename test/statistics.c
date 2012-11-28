#include <stdio.h>

/*
 * 统计网页字符频率，以优化匹配效率
 */
 void sort(int [], int);
 void print_sort(int [], int);

int main(int argc, char const *argv[])
{
    int char8[256] = { 0 };    // 0x00~0xfe

    if (argc != 2)
    {
        return 0;
    }

    FILE *file = fopen(argv[1], "r");
    int num;
    while ((num = getc(file)) != EOF)
    {
        char8[num]++;
    }

    // 排序
    print_sort(char8, 256);

    // int i;
    // for (i = 0; i < 256; ++i)
    // {
    //     if (i >= ' ' && i <= '~')
    //     {
    //         printf("%c:\t%d\n", i, char8[i]);
    //     }
    //     else
    //     {
    //         printf("%d:\t%d\n", i, char8[i]);
    //     }
    // }

    return 0;
}

void sort(int arr[], int count)
{
    int i, j, tmp;
    int iswap;
    for (i = 0; i < count; ++i)
    {
        iswap = 0;
        for (j = count - 1; j > i; --j)
        {
            if (arr[j - 1] < arr[j])
            {
                tmp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = tmp;

                iswap = 1;
            }
        }

        if (iswap == 0)
        {
            break;
        }
    }
}

void print_sort(int arr[], int count)
{
    int loc_max = 0;
    int i, j;
    for (i = 0; i < count; ++i)
    {
        for (j = 0; j < count; ++j)
        {
            if (arr[j] > arr[loc_max])
            {
                loc_max = j;
            }
        }

        if (arr[loc_max] == 0)
        {
            return;
        }

        if (loc_max >= ' ' && loc_max <= '~')
        {
            printf(" %c :\t%d\n", loc_max, arr[loc_max]);
        }
        else
        {
            printf("[%d]:\t%d\n", loc_max, arr[loc_max]);
        }

        arr[loc_max] = 0;
    }
}
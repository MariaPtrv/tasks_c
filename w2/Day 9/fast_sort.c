#include <stdio.h>

#define SIZE 10

int input(int *);
void output(int *, int *, int);
void print_arr(int *a);

void copy_arr(int *, int *);

void quick_sort(int *, int, int);
void swap(int *, int *);
int partition(int *, int, int);

void merge_sort(int *, int, int);
void merge_arr(int *, int, int, int);

int main()
{
    int a[SIZE], b[SIZE], error;
    char c;

    error = input(a);
    if (!error)
    {
        copy_arr(a, b);
        //quick_sort(a, 0, SIZE);
        merge_sort(b, 0, SIZE);
    }
    output(a, b, error);
    return 0;
}

int input(int *a)
{
    int error = 0;
    char c;
    for (int i = 0; i < SIZE && !error; i++)
    {
        if (scanf("%d%c", &(a[i]), &c) != 2)
        {
            error = 1;
        }
        else if (i == SIZE - 1 && c != '\n')
        {
            error = 1;
        }
        else if (i < SIZE - 1 && c != ' ')
        {
            error = 1;
        }
    }

    return error;
}

void merge_sort(int *a, int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        merge_sort(a, l, m);
        merge_sort(a, m + 1, r);

        merge_arr(a, l, m, r);
    }
}

void merge_arr(int *a, int l, int m, int r)
{
    int tempL[SIZE], tempR[SIZE];
    int n1 = m - l + 1, n2 = r - m, c = l;

    for (int i = 0; i < n1; i++)
    {
        tempL[i] = a[c + i];
    }
    for (int i = 0; i < n2; i++)
    {
        tempR[i] = a[m + 1 + i];
    }

    int i = 0, j = 0;
    while (i < n1 && j < n2)
    {
        if (tempL[i] <= tempR[j])
        {
            a[c + i] = tempL[i];
            i++;
        }
        else
        {
            a[m + 1 + j] = tempR[j];
            j++;
        }
    }

    while (i < n1)
    {
        a[c] = tempL[i];
        i++;
        c++;
    }

    while (j < n2)
    {
        a[c] = tempR[j];
        j++;
        c++;
    }
}

void quick_sort(int *a, int l, int r)
{
    if (l < r)
    {
        int pi = partition(a, l, r);
        quick_sort(a, l, pi - 1);
        quick_sort(a, pi + 1, r);
    }
}

int partition(int arr[], int l, int r)
{

    int p = arr[l], i = l, j = r;

    while (i < j)
    {
        while (arr[i] <= p && i <= r - 1)
        {
            i++;
        }
        while (arr[j] > p && j >= l + 1)
        {
            j--;
        }
        if (i < j)
        {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[l], &arr[j]);
    return j;
}

void copy_arr(int *from, int *to)
{
    for (int i = 0; i < SIZE; i++)
    {
        to[i] = from[i];
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void output(int *a, int *b, int error)
{
    if (!error)
    {
        print_arr(a);
        printf("\n");
        print_arr(b);
    }
    else
    {
        printf("n/a");
    }
}

void print_arr(int *a)
{
    for (int i = 0; i < SIZE - 1; i++, a++)
    {
        printf("%d ", *a);
    }
    printf("%d", *a);
}
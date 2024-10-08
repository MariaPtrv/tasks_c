#include <stdio.h>

#define SIZE 10

int input(int *);
void sort(int *);
void output(int *, int);

int main()
{
    int a[SIZE], error;
    char c;

    error = input(a);
    if (!error)
    {
        sort(a);
    }
    output(a, error);
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

void sort(int *a)
{
    int temp;
    for (int i = 0; i < SIZE - 1; i++)
    {
        for (int j = 0; j < SIZE - 1 - i; j++)
        {
            if (a[j + 1] < a[j])
            {
                temp = a[j + 1];
                a[j + 1] = a[j];
                a[j] = temp;
            }
        }
    }
}

void output(int *a, int error)
{
    if (!error)
    {
        for (int i = 0; i < SIZE - 1; i++, a++)
        {
            printf("%d ", *a);
        }
        printf("%d", *a);
    }
    else
    {
        printf("n/a");
    }
}
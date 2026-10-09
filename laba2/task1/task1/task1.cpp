
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#define MAX_SIZE 10000

int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
long long c[MAX_SIZE][MAX_SIZE];

int main(void)
{
	setlocale(LC_ALL, "Russian");
    int n;
    int i, j, r;
    clock_t start, end;
    double time_program, time_multiply;

    printf("Введите размер матрицы (Мин - 1, макс - 10000): ");
    scanf_s("%d", &n);

    if (n < 1 || n > MAX_SIZE) {
        printf("Недопустимое значение!\n");
        return 1;
    }

    srand((unsigned)time(NULL));

    start = clock();

    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            a[i][j] = rand() % 100 + 1;
        }
    }

    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            b[i][j] = rand() % 100 + 1;
        }
    }

    end = clock();
    time_program = (double)(end - start) / CLOCKS_PER_SEC;

    
    start = clock();

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c[i][j] = 0;

            for (r = 0; r < n; r++) {
                c[i][j] += (long long)a[i][r] * b[r][j];
            }
        }
    }

    end = clock();
    time_multiply = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nРазмер матрицы: %d x %d\n", n, n);
    printf("Время заполнения матриц: %.6f сек\n",
        time_program);
    printf("Время перемножения матриц: %.6f сек\n",
        time_multiply);

    return 0;
}
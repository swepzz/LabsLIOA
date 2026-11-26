#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

int showBinaryOperations(int **first, int firstSize, int **second, int secondSize);

void generateMatrix(int** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            matrix[i][j] = rand() % 2;
            matrix[j][i] = matrix[i][j];
        }
        matrix[i][i] = 0;
    }
}

void generatesecondMatrix(int** matrix, int N2) {
    for (int i = 0; i < N2; i++) {
        for (int j = i + 1; j < N2; j++) {
            matrix[i][j] = rand() % 2;
            matrix[j][i] = matrix[i][j];
        }
        matrix[i][i] = 0;
    }
}

int** identifyVertices(int** matrix, int N, int u, int v) {
    int newN = N - 1;

    int** newMatrix = (int**)malloc(newN * sizeof(int*));

    if (newMatrix == NULL) {
        return NULL;
    }

    for (int i = 0; i < newN; i++) {
        newMatrix[i] = (int*)calloc(newN, sizeof(int));

        if (newMatrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(newMatrix[j]);
            }

            free(newMatrix);
            return NULL;
        }
    }

    int newI = 0;

    for (int i = 0; i < N; i++) {
        if (i == u || i == v) {
            continue;
        }

        int newJ = 0;

        for (int j = 0; j < N; j++) {
            if (j == u || j == v) {
                continue;
            }

            newMatrix[newI][newJ] = matrix[i][j];
            newJ++;
        }

        newI++;
    }

    int z = newN - 1;
    newI = 0;

    for (int i = 0; i < N; i++) {
        if (i == u || i == v) {
            continue;
        }

        if (matrix[i][u] == 1 || matrix[i][v] == 1) {
            newMatrix[newI][z] = 1;
            newMatrix[z][newI] = 1;
        }

        newI++;
    }

    return newMatrix;
}
int** contractEdge(int** matrix, int N, int u, int v){
    if (matrix[u][v] != 1) {
        return NULL;
    }
    return identifyVertices(matrix, N, u, v);
}


int** splitVertex(int** matrix, int N, int u) {
    int newN = N + 1;

    int** newMatrix = (int**)calloc(newN, sizeof(int*));

    if (newMatrix == NULL) {
        return NULL;
    }

    for (int i = 0; i < newN; i++) {
        newMatrix[i] = (int*)calloc(newN, sizeof(int));

        if (newMatrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(newMatrix[j]);
            }

            free(newMatrix);
            return NULL;
        }
    }

    int count = 0;

    for (int i = 0; i < N; i++) {
        if (i == u) {
            continue;
        }

        for (int j = 0; j < N; j++) {
            if (j == u) {
                continue;
            }

            newMatrix[i][j] = matrix[i][j];
        }

        if (matrix[u][i] == 1) {
            if (count % 2 == 0) {
                newMatrix[u][i] = 1;
                newMatrix[i][u] = 1;
            }
            else {
                newMatrix[N][i] = 1;
                newMatrix[i][N] = 1;
            }

            count++;
        }
    }

    newMatrix[u][N] = 1;
    newMatrix[N][u] = 1;

    return newMatrix;
}

int main() {
    srand((unsigned int)time(NULL));
    setlocale(LC_ALL, ".UTF8");

    int N, N2;

    printf("Введите количество вершин графа: ");
    scanf("%d", &N);

    printf("Введите количество вершин 2 графа: ");
    scanf("%d", &N2);

    if (N < 2 || N2 < 2) {
        printf("Ошибка: в каждом графе должно быть минимум 2 вершины.\n");
        return 1;
    }

    int** matrix = (int**)malloc(N * sizeof(int*));
    int** matrix2 = (int**)malloc(N2 * sizeof(int*));

    if (matrix == NULL || matrix2 == NULL) {
        printf("Ошибка выделения памяти.\n");
        free(matrix);
        free(matrix2);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        matrix[i] = (int*)malloc(N * sizeof(int));

        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            free(matrix2);
            return 1;
        }
    }

    for (int i = 0; i < N2; i++) {
        matrix2[i] = (int*)malloc(N2 * sizeof(int));

        if (matrix2[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix2[j]);
            }
            free(matrix2);

            for (int j = 0; j < N; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return 1;
        }
    }

    generateMatrix(matrix, N);
    generatesecondMatrix(matrix2, N2);

    printf("\nМатрица смежности первого графа:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nМатрица смежности второго графа:\n");

    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N2; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }

    if (!showBinaryOperations(matrix, N, matrix2, N2)) {
        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        for (int i = 0; i < N2; i++) {
            free(matrix2[i]);
        }
        free(matrix);
        free(matrix2);
        return 1;
    }

    int u, v;
    int** newMatrix = NULL;
    int** newMatrix2 = NULL;

    printf("\nОтождествление вершин первого графа.\n");
    printf("Введите номера двух вершин: ");
    scanf("%d %d", &u, &v);

    if (u < 1 || u > N || v < 1 || v > N || u == v) {
        printf("Ошибка: введены некорректные номера вершин.\n");
        return 1;
    }

    newMatrix = identifyVertices(matrix, N, u - 1, v - 1);

    if (newMatrix == NULL) {
        printf("Ошибка создания новой матрицы.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    matrix = newMatrix;
    N--;

    printf("\nМатрица первого графа после отождествления:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nСтягивание ребра первого графа.\n");
    printf("Введите номера двух вершин: ");
    scanf("%d %d", &u, &v);

    if (u < 1 || u > N || v < 1 || v > N || u == v) {
        printf("Ошибка: введены некорректные номера вершин.\n");
        return 1;
    }
    else if (matrix[u - 1][v - 1] != 1) {
        printf("Ошибка: введенные вершины не соединены ребром.\n");
        return 1;
    }
    else {
        newMatrix = contractEdge(matrix, N, u - 1, v - 1);

        if (newMatrix == NULL) {
            printf("Ошибка создания новой матрицы.\n");
            return 1;
        }

        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        free(matrix);

        matrix = newMatrix;
        N--;

        printf("\nМатрица первого графа после стягивания ребра:\n");

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }

    printf("\nРасщепление вершины первого графа.\n");
    printf("Введите номер вершины: ");
    scanf("%d", &u);

    if (u < 1 || u > N) {
        printf("Ошибка: введен некорректный номер вершины.\n");
        return 1;
    }

    newMatrix = splitVertex(matrix, N, u - 1);

    if (newMatrix == NULL) {
        printf("Ошибка создания новой матрицы.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    matrix = newMatrix;
    N++;

    printf("\nМатрица первого графа после расщепления вершины:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nОтождествление вершин второго графа.\n");
    printf("Введите номера двух вершин: ");
    scanf("%d %d", &u, &v);

    if (u < 1 || u > N2 || v < 1 || v > N2 || u == v) {
        printf("Ошибка: введены некорректные номера вершин.\n");
        return 1;
    }

    newMatrix2 = identifyVertices(matrix2, N2, u - 1, v - 1);

    if (newMatrix2 == NULL) {
        printf("Ошибка создания новой матрицы.\n");
        return 1;
    }

    for (int i = 0; i < N2; i++) {
        free(matrix2[i]);
    }
    free(matrix2);

    matrix2 = newMatrix2;
    N2--;

    printf("\nМатрица второго графа после отождествления:\n");

    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N2; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }

    printf("\nСтягивание ребра второго графа.\n");
    printf("Введите номера двух вершин: ");
    scanf("%d %d", &u, &v);

    if (u < 1 || u > N2 || v < 1 || v > N2 || u == v) {
        printf("Ошибка: введены некорректные номера вершин.\n");
        return 1;
    }
    else if (matrix2[u - 1][v - 1] != 1) {
        printf("Ошибка: введенные вершины не соединены ребром.\n");
        return 1;
    }
    else {
        newMatrix2 = contractEdge(matrix2, N2, u - 1, v - 1);

        if (newMatrix2 == NULL) {
            printf("Ошибка создания новой матрицы.\n");
            return 1;
        }

        for (int i = 0; i < N2; i++) {
            free(matrix2[i]);
        }
        free(matrix2);

        matrix2 = newMatrix2;
        N2--;

        printf("\nМатрица второго графа после стягивания ребра:\n");

        for (int i = 0; i < N2; i++) {
            for (int j = 0; j < N2; j++) {
                printf("%d ", matrix2[i][j]);
            }
            printf("\n");
        }
    }

    printf("\nРасщепление вершины второго графа.\n");
    printf("Введите номер вершины: ");
    scanf("%d", &u);

    if (u < 1 || u > N2) {
        printf("Ошибка: введен некорректный номер вершины.\n");
        return 1;
    }

    newMatrix2 = splitVertex(matrix2, N2, u - 1);

    if (newMatrix2 == NULL) {
        printf("Ошибка создания новой матрицы.\n");
        return 1;
    }

    for (int i = 0; i < N2; i++) {
        free(matrix2[i]);
    }
    free(matrix2);

    matrix2 = newMatrix2;
    N2++;

    printf("\nМатрица второго графа после расщепления вершины:\n");

    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N2; j++) {
            printf("%d ", matrix2[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    for (int i = 0; i < N2; i++) {
        free(matrix2[i]);
    }
    free(matrix2);

    return 0;
}

static int hasEdge(int **matrix, int size, int u, int v) {
    return u < size && v < size && matrix[u][v] != 0;
}

static void printCombinedMatrix(int **first, int firstSize, int **second,
                                int secondSize, int size, int minimumCount) {
    for (int u = 0; u < size; u++) {
        for (int v = 0; v < size; v++) {
            int count = hasEdge(first, firstSize, u, v)
                      + hasEdge(second, secondSize, u, v);
            printf("%d ", count >= minimumCount);
        }
        putchar('\n');
    }
}

static int ringEdge(int **first, int firstSize, int **second,
                    int secondSize, int u, int v) {
    return hasEdge(first, firstSize, u, v) != hasEdge(second, secondSize, u, v);
}

static int printRingSum(int **first, int firstSize, int **second,
                        int secondSize, int size) {
    long selected;
    for (;;) {
        char input[32];
        char *end;
        printf("Введите номер вершины для проверки (1–%d): ", size);
        if (scanf("%31s", input) != 1) {
            putchar('\n');
            return 0;
        }
        selected = strtol(input, &end, 10);
        if (*end == '\0' && selected >= 1 && selected <= size) {
            break;
        }
        puts("Ошибка: введите корректный номер вершины.");
    }

    int *vertices = (int *)malloc((size_t)size * sizeof *vertices);
    if (vertices == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для кольцевой суммы.\n");
        return 0;
    }

    int count = 0;
    int selectedFound = 0;
    for (int u = 0; u < size; u++) {
        for (int v = 0; v < size; v++) {
            if (ringEdge(first, firstSize, second, secondSize, u, v)) {
                if (u + 1 == selected) {
                    selectedFound = 1;
                } else {
                    vertices[count++] = u;
                }
                break;
            }
        }
    }

    if (selectedFound) {
        printf("Вершина %ld есть в кольцевой сумме и удалена вместе со своими рёбрами.\n",
               selected);
    } else {
        printf("Вершины %ld нет в кольцевой сумме.\n", selected);
    }
    puts("Результирующий граф:");

    if (count == 0) {
        puts("Граф пуст.");
    } else {
        printf("Номера вершин по порядку строк и столбцов:");
        for (int i = 0; i < count; i++) {
            printf(" %d", vertices[i] + 1);
        }
        putchar('\n');

        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count; j++) {
                int edge = ringEdge(first, firstSize, second, secondSize,
                                    vertices[i], vertices[j]);
                printf("%d ", edge);
            }
            putchar('\n');
        }
    }
    free(vertices);
    return 1;
}

int showBinaryOperations(int **first, int firstSize, int **second, int secondSize) {
    int smallerSize = firstSize < secondSize ? firstSize : secondSize;
    int largerSize = firstSize > secondSize ? firstSize : secondSize;

    puts("\nОбъединение G1 и G2:");
    printCombinedMatrix(first, firstSize, second, secondSize, largerSize, 1);

    puts("Пересечение G1 и G2:");
    printCombinedMatrix(first, firstSize, second, secondSize, smallerSize, 2);

    puts("Кольцевая сумма G1 и G2:");
    return printRingSum(first, firstSize, second, secondSize, largerSize);
}

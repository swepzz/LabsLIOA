#include <algorithm>
#include <clocale>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#pragma execution_character_set("utf-8")

void shell(int* items, int count)
{
    int gaps[5] = { 9, 5, 3, 2, 1 };

    for (int k = 0; k < 5; k++) {
        int gap = gaps[k];

        for (int i = gap; i < count; i++) {
            int x = items[i];
            int j;

            for (j = i - gap; j >= 0 && x < items[j]; j -= gap) {
                items[j + gap] = items[j];
            }

            items[j + gap] = x;
        }
    }
}

void qs(int* items, int left, int right)
{
    int i = left;
    int j = right;
    int x = items[(left + right) / 2];

    do {
        while (items[i] < x && i < right) {
            i++;
        }

        while (x < items[j] && j > left) {
            j--;
        }

        if (i <= j) {
            int y = items[i];
            items[i] = items[j];
            items[j] = y;
            i++;
            j--;
        }
    } while (i <= j);

    if (left < j) {
        qs(items, left, j);
    }

    if (i < right) {
        qs(items, i, right);
    }
}

int compareIntegers(const void* first, const void* second)
{
    int a = *static_cast<const int*>(first);
    int b = *static_cast<const int*>(second);

    return (a > b) - (a < b);
}

double measureSort(const std::vector<int>& source, int algorithm)
{
    std::vector<int> items = source;

    clock_t start = clock();

    if (algorithm == 1) {
        shell(items.data(), static_cast<int>(items.size()));
    }
    else if (algorithm == 2) {
        qs(items.data(), 0, static_cast<int>(items.size()) - 1);
    }
    else {
        qsort(
            items.data(),
            items.size(),
            sizeof(int),
            compareIntegers
        );
    }

    clock_t end = clock();

    if (!std::is_sorted(items.begin(), items.end())) {
        std::cout << "Ошибка сортировки!\n";
    }

    return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

std::vector<int> createRandomArray(int size)
{
    std::vector<int> items(size);

    for (int& item : items) {
        item = std::rand() % 100000;
    }

    return items;
}

std::vector<int> createAscendingArray(int size)
{
    std::vector<int> items(size);

    for (int i = 0; i < size; i++) {
        items[i] = i;
    }

    return items;
}

std::vector<int> createDescendingArray(int size)
{
    std::vector<int> items(size);

    for (int i = 0; i < size; i++) {
        items[i] = size - i;
    }

    return items;
}

std::vector<int> createCombinedArray(int size)
{
    std::vector<int> items(size);
    int middle = size / 2;

    for (int i = 0; i < middle; i++) {
        items[i] = i;
    }

    for (int i = middle; i < size; i++) {
        items[i] = size - i;
    }

    return items;
}

void printResult(
    const std::string& arrayType,
    const std::vector<int>& source
)
{
    double shellTime = measureSort(source, 1);
    double quickTime = measureSort(source, 2);
    double qsortTime = measureSort(source, 3);

    std::cout
        << std::left << std::setw(35) << arrayType
        << std::right << std::setw(15) << shellTime
        << std::setw(18) << quickTime
        << std::setw(15) << qsortTime
        << '\n';
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int size;

    std::cout << "Введите размер массива: ";
    std::cin >> size;

    if (size < 1 || size > 100000) {
        std::cout << "Размер должен быть от 1 до 100000.\n";
        return 1;
    }

    std::vector<int> randomArray = createRandomArray(size);
    std::vector<int> ascendingArray = createAscendingArray(size);
    std::vector<int> descendingArray = createDescendingArray(size);
    std::vector<int> combinedArray = createCombinedArray(size);

    std::cout << "\nРазмер массива: " << size << "\n";
    std::cout << "Время указано в секундах.\n\n";

    std::cout
        << std::left << std::setw(35) << "Набор данных"
        << std::right << std::setw(15) << "Шелл"
        << std::setw(18) << "Быстрая"
        << std::setw(15) << "qsort"
        << '\n';

    std::cout << std::string(83, '-') << '\n';

    std::cout << std::fixed << std::setprecision(6);

    printResult("Случайные значения", randomArray);
    printResult("Возрастающая последовательность", ascendingArray);
    printResult("Убывающая последовательность", descendingArray);
    printResult("Половина возрастает, половина убывает", combinedArray);

    return 0;
}
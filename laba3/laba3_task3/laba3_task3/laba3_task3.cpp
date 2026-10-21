#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <windows.h>
#include <stdarg.h>

#pragma execution_character_set("utf-8")

struct node {
    char name[256];
    struct node* next;
};

struct node* top = NULL;

int consolePrintf(const char* format, ...)
{
    char utf8Buffer[4096];
    wchar_t wideBuffer[4096];
    va_list arguments;
    int byteCount;
    int wideCount;
    DWORD written;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    va_start(arguments, format);
    byteCount = vsnprintf(utf8Buffer, sizeof(utf8Buffer), format, arguments);
    va_end(arguments);

    if (byteCount < 0) {
        return byteCount;
    }

    wideCount = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8Buffer,
        -1,
        wideBuffer,
        (int)(sizeof(wideBuffer) / sizeof(wideBuffer[0]))
    );

    if (wideCount > 0 && WriteConsoleW(
        output,
        wideBuffer,
        wideCount - 1,
        &written,
        NULL
    )) {
        return (int)written;
    }

    return fputs(utf8Buffer, stdout);
}

#define printf consolePrintf

void add()
{
    struct node* p = (struct node*)malloc(sizeof(struct node));

    if (p == NULL) {
        printf("Ошибка выделения памяти\n");
        return;
    }

    printf("Введите название объекта: ");
    scanf("%255s", p->name);

    p->next = top;
    top = p;

    printf("Элемент добавлен в стек\n");
}

void del()
{
    struct node* temp;

    if (top == NULL) {
        printf("Стек пуст\n");
        return;
    }

    temp = top;
    top = top->next;

    printf("Удален элемент: %s\n", temp->name);
    free(temp);
}

void review()
{
    struct node* temp = top;

    if (top == NULL) {
        printf("Стек пуст\n");
        return;
    }

    printf("Содержимое стека (от вершины к основанию):\n");
    while (temp != NULL) {
        printf("Имя: %s\n", temp->name);
        temp = temp->next;
    }
}

void moveToTopByName()
{
    char name[256];
    int moved = 0;
    struct node* current;
    struct node* next;
    struct node* matchingHead = NULL;
    struct node* matchingTail = NULL;
    struct node* otherHead = NULL;
    struct node* otherTail = NULL;

    if (top == NULL) {
        printf("Стек пуст\n");
        return;
    }

    printf("Введите имя объекта: ");
    scanf("%255s", name);
    current = top;

    while (current != NULL) {
        next = current->next;
        current->next = NULL;

        if (strcmp(current->name, name) == 0) {
            if (matchingHead == NULL) {
                matchingHead = current;
            }
            else {
                matchingTail->next = current;
            }
            matchingTail = current;
            moved++;
        }
        else {
            if (otherHead == NULL) {
                otherHead = current;
            }
            else {
                otherTail->next = current;
            }
            otherTail = current;
        }

        current = next;
    }

    if (matchingHead == NULL) {
        top = otherHead;
        printf("Элементы с именем %s не найдены\n", name);
        return;
    }

    matchingTail->next = otherHead;
    top = matchingHead;
    printf("Элементы с именем %s перенесены на вершину: %d\n", name, moved);
}

void reverseQueue()
{
    struct node* previous = NULL;
    struct node* current = top;
    struct node* next;

    if (top == NULL) {
        printf("Очередь пуста\n");
        return;
    }

    while (current != NULL) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    top = previous;
    printf("Очередь развернута\n");
}

void clear()
{
    struct node* temp;

    while (top != NULL) {
        temp = top;
        top = top->next;
        free(temp);
    }
}

int main()
{
    int choice;
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    do {
        printf("\n1. Добавить элемент\n");
        printf("2. Удалить верхний элемент\n");
        printf("3. Просмотреть стек\n");
        printf("4. Найти элементы по имени и перенести на вершину\n");
        printf("5. Развернуть очередь\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add();
            break;
        case 2:
            del();
            break;
        case 3:
            review();
            break;
        case 4:
            moveToTopByName();
            break;
        case 5:
            reverseQueue();
            break;
        case 0:
            clear();
            printf("Выход\n");
            break;
        default:
            printf("Неверный выбор\n");
        }
    } while (choice != 0);

    return 0;
}
